#pragma once

#include <cstdint>
#include <atomic>
#include <vector>
#include <functional>
#include <thread>
#include <memory>

#include <tgbot/tgbot.h>

#include "../../Services/Configs/TgBotConfig.hpp"
#include "../../Services/Configs/TgBotResponseConfig.hpp"
#include "../../Services/Service.hpp"

#include "../ClientSubscriber.hpp"
#include "../ClientBrocker.hpp"

#include "../../StringUtility.hpp"

class TgBotSubscriber : public ClientSubscriber
{
public:
    TgBotSubscriber(ClientBrocker &brocker) : ClientSubscriber{ClientType::telegram}, brocker_{brocker}, bot_{Service::config.TG_BOT_KEY}
    {
        brocker_.subscribe(*this);
        usersInfo_ = loadTgBotConfig(Service::config.TG_BOT_CONFIG);
        responseConfig_ = loadTgBotResponseConfig(Service::config.TG_BOT_RESPONSE_CONFIG);

        initKeyboard();
        initResponses();

        std::lock_guard lg(mut_);
        Service::log.log("Bot started. Bot name: " + bot_.getApi().getMe()->username, crow::LogLevel::Info);
    }

    void sendMessage(const Message &msg) override
    {
        if (msg.type == MessageType::audio)
        {
            // TODO
        }
        std::string sMsg;
        if (msg.type == MessageType::text)
            sMsg = '<' + msg["userName"] + '>' + ' ' + msg["text"];
        else if (msg.type == MessageType::achievement)
            sMsg = msg["userName"] + ' ' + responseConfig_.achievement + ' ' + msg["text"];
        else if (msg.type == MessageType::left)
            sMsg = msg["userName"] + ' ' + responseConfig_.left_the_game;
        else if (msg.type == MessageType::join)
            sMsg = msg["userName"] + ' ' + responseConfig_.join_the_game;
            
        sendMessageToAllTgExcept(sMsg);
    }

    void stop() override
    {
        Service::log.log("Start to stop Tg bot.", crow::LogLevel::Info);
        saveTgBotConfig(usersInfo_, Service::config.TG_BOT_CONFIG);
        isWorking_ = false;
        if(runThreadPtr_ && runThreadPtr_->joinable())
            runThreadPtr_->join();
        Service::log.log("Tg bot stopped.", crow::LogLevel::Info);
    }

    void multithreadedRun()
    {
        runThreadPtr_ = std::make_unique<std::thread>(&TgBotSubscriber::run, this);
    }

    void run()
    {
        TgBot::TgLongPoll longPoll(bot_);
        while (isWorking_)
        {
            longPoll.start();
        }
    }
    
private:
    // посредник
    ClientBrocker &brocker_;

    // Бот
    TgBot::Bot bot_;
    TgBot::ReplyKeyboardMarkup::Ptr keyboard_;

    // Информация о чатах
    TgBotConfig usersInfo_;
    TgBotResponseConfig responseConfig_;

    std::atomic<bool> isWorking_ = true;

    std::unique_ptr<std::thread> runThreadPtr_ = nullptr;

#pragma region Static Methods

private:
    // проверка, что чат - сообщество и не general
    static bool notGeneralInSuperGroup(TgBot::Message::Ptr message)
    {
        return message->chat != nullptr &&
               message->chat->type == TgBot::Chat::Type::Supergroup &&
               message->chat->isForum &&
               message->isTopicMessage;
    }

    static TgBot::KeyboardButton::Ptr makeButton(const std::string &text)
    {
        TgBot::KeyboardButton::Ptr res(new TgBot::KeyboardButton);
        res->text = text;
        return std::move(res);
    }

#pragma endregion

#pragma region Initializing

private:
    void initKeyboard()
    {
        keyboard_ = TgBot::ReplyKeyboardMarkup::Ptr(new TgBot::ReplyKeyboardMarkup);
        keyboard_->resizeKeyboard = true;
        keyboard_->oneTimeKeyboard = false;
        // clang-format off
        keyboard_->keyboard =
        {
            {makeButton("/start"),  makeButton("/help")       },
            {makeButton("/online"), makeButton("/renew")      },
            {makeButton("/update"), makeButton("/registrate") }
        };
        // clang-format onn
    }

    void initResponses()
    {
        std::lock_guard lg(mut_);
        // clang-format off
        bot_.getEvents().onCommand("start",       [this](TgBot::Message::Ptr message) { start(message);          });
        bot_.getEvents().onCommand("help",        [this](TgBot::Message::Ptr message) { help(message);           });
        bot_.getEvents().onCommand("online",      [this](TgBot::Message::Ptr message) { online(message);         });
        bot_.getEvents().onCommand("renew",       [this](TgBot::Message::Ptr message) { renew(message);          });
        bot_.getEvents().onCommand("update",      [this](TgBot::Message::Ptr message) { update(message);         });
        bot_.getEvents().onCommand("registrate",  [this](TgBot::Message::Ptr message) { registrate(message);     });
        bot_.getEvents().onCommand("stop",        [this](TgBot::Message::Ptr message) { stop(message);           });
        bot_.getEvents().onNonCommandMessage(     [this](TgBot::Message::Ptr message) { processMessage(message); });
        // clang-format on
    }

#pragma endregion

#pragma region Events

private:
    bool isAdmin(TgBot::Message::Ptr message) const
    {
        if (message->chat->type != TgBot::Chat::Type::Private && message->chat->type != TgBot::Chat::Type::Supergroup && message->chat->type != TgBot::Chat::Type::Group)
            return false;
        return usersInfo_.ADMINS_ID.count(message->from->id);
        // auto admins = bot_.getApi().getChatAdministrators(message->chat->id);
        // for (const auto &admin : admins)
        //     if (admin->user->id == message->from->id)
        //         return true;
        // return false;
    }
    bool isChatIsOpen(const int64_t id) const
    {
        return usersInfo_.CHATS_ID.count(id);
    }

    void stop(TgBot::Message::Ptr message)
    {
        if(!notGeneralInSuperGroup(message))
            return;
        if (!isAdmin(message))
            return sendMessage(message->chat->id, responseConfig_.not_admin);
        sendMessage(message->chat->id, responseConfig_.stoping_run);
        brocker_.stop();
    }
    void start(TgBot::Message::Ptr message) const
    {
        if(!notGeneralInSuperGroup(message))
            return;
        sendMessage(message->chat->id, responseConfig_.start_command);
    }
    void help(TgBot::Message::Ptr message) const
    {
        if(!notGeneralInSuperGroup(message))
            return;
        sendMessage(message->chat->id, responseConfig_.help_comand);
    }
    void online(TgBot::Message::Ptr message) const
    {
        if(!notGeneralInSuperGroup(message))
            return;
        // TODO
        sendMessage(message->chat->id, responseConfig_.online_comand_fatal);
    }
    void renew(TgBot::Message::Ptr message) const
    {
        if(!notGeneralInSuperGroup(message))
            return;
        // TODO
        sendMessage(message->chat->id, responseConfig_.renew_command_fatal);
    }
    void update(TgBot::Message::Ptr message)
    {
        if(!notGeneralInSuperGroup(message))
            return;
        if (!isAdmin(message))
            return sendMessage(message->chat->id, responseConfig_.not_admin);

        usersInfo_ = loadTgBotConfig(Service::config.TG_BOT_CONFIG);
        responseConfig_ = loadTgBotResponseConfig(Service::config.TG_BOT_RESPONSE_CONFIG);
        sendMessage(message->chat->id, responseConfig_.update);
    }
    void registrate(TgBot::Message::Ptr message)
    {
        if(!notGeneralInSuperGroup(message))
            return;
        if (!isAdmin(message))
            return sendMessage(message->chat->id, responseConfig_.not_admin);
        usersInfo_.CHATS_ID.insert(message->chat->id);
        saveTgBotConfig(usersInfo_, Service::config.TG_BOT_CONFIG);
        sendMessage(message->chat->id, responseConfig_.registrate_command);
    }

    void processMessage(TgBot::Message::Ptr message)
    {
        // (Сообщение пустой и нет голосового ) или чат закрыт
        if ((message->text.empty() && !message->voice) || !isChatIsOpen(message->chat->id))
            return;
        if(!notGeneralInSuperGroup(message))
            return;
        if (message->voice)
        {
            Message msg;
            msg.from = type_;
            msg.to = ClientType::any;
            msg.fromId = id_;
            msg.type = MessageType::audio;
            msg.msg["userName"] = message->from->username;
            msg.msg["audioFile"] = bot_.getApi().getFile(message->voice->fileId)->filePath;

            brocker_.addMessage(msg);
            sendAudioToAllTgExcept(bot_.getApi().getFile(message->voice->fileId)->filePath, message->chat->id);
        }
        else
        {
            Message msg;
            msg.from = type_;
            msg.to = ClientType::any;
            msg.fromId = id_;
            msg.type = MessageType::text;
            msg.msg["userName"] = message->from->username;
            msg.msg["text"] = message->text;

            brocker_.addMessage(msg);

            std::string textMessage = '<' + message->from->username + '>' + ' ' + message->text;
            sendMessageToAllTgExcept(textMessage, message->chat->id);
        }
    }

#pragma endregion

#pragma region Sending Messages

private:
    void sendMessage(const int64_t chatId, const std::string &message) const
    {
        std::unique_lock<std::mutex> lg(mut_);

#if defined(_WIN32) || defined(_WIN64)
        bool thirdParam = false;
#else
        decltype(nullptr) thirdParam = nullptr;
#endif // WIN
        bot_.getApi().sendMessage(chatId, message, thirdParam, 0/*, keyboard_*/);
    }
    void sendMessageToAllTgExcept(const std::string &message, const std::int64_t exceptIdTg = 0) const
    {
        for (const auto &chatId : usersInfo_.CHATS_ID)
            if (chatId != exceptIdTg)
                sendMessage(chatId, message);
    }
    void sendAudioToAllTgExcept(const std::string &filePath, const std::int64_t exceptIdTg = 0) const
    {
        for (const auto &chatId : usersInfo_.CHATS_ID)
            if (chatId != exceptIdTg)
            {
                // TODO
            }
    }

#pragma endregion

};