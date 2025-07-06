#pragma once

#include <cstdint>
#include <atomic>
#include <vector>
#include <functional>

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

        Service::log.log("Bot started. Bot name: " + bot_.getApi().getMe()->username, crow::LogLevel::Info);
    }
    ~TgBotSubscriber()
    {
        saveTgBotConfig(usersInfo_, Service::config.TG_BOT_CONFIG);
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

    void runNoAsynch()
    {
        while(isWorking_)
            longPoll();
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
        // clang-format off
        bot_.getEvents().onCommand("start",       [this](TgBot::Message::Ptr message) { start(message);          });
        bot_.getEvents().onCommand("help",        [this](TgBot::Message::Ptr message) { help(message);           });
        bot_.getEvents().onCommand("online",      [this](TgBot::Message::Ptr message) { online(message);         });
        bot_.getEvents().onCommand("renew",       [this](TgBot::Message::Ptr message) { renew(message);          });
        bot_.getEvents().onCommand("update",      [this](TgBot::Message::Ptr message) { update(message);         });
        bot_.getEvents().onCommand("registrate",  [this](TgBot::Message::Ptr message) { registrate(message);     });
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

    void start(TgBot::Message::Ptr message) const
    {
        sendMessage(message->chat->id, responseConfig_.start_command);
    }
    void help(TgBot::Message::Ptr message) const
    {
        sendMessage(message->chat->id, responseConfig_.help_comand);
    }
    void online(TgBot::Message::Ptr message) const
    {
        // TODO
        sendMessage(message->chat->id, responseConfig_.online_comand_fatal);
    }
    void renew(TgBot::Message::Ptr message) const
    {
        // TODO
        sendMessage(message->chat->id, responseConfig_.renew_command_fatal);
    }
    void update(TgBot::Message::Ptr message)
    {
        if (!isAdmin(message))
            return sendMessage(message->chat->id, responseConfig_.not_admin);

        usersInfo_ = loadTgBotConfig(Service::config.TG_BOT_CONFIG);
        responseConfig_ = loadTgBotResponseConfig(Service::config.TG_BOT_RESPONSE_CONFIG);
        sendMessage(message->chat->id, responseConfig_.update);
    }
    void registrate(TgBot::Message::Ptr message)
    {
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
        bot_.getApi().sendMessage(chatId, message, false, 0, keyboard_);
#else
        bot_.getApi().sendMessage(chatId, message, nullptr, 0, keyboard_);
#endif // WIN
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

#pragma region Runing

    void longPoll()
    {
        TgBot::TgLongPoll longPoll(bot_);
        while (isWorking_)
        {
            Service::log.log("Long poll started", crow::LogLevel::Debug);
            longPoll.start();
        }
    }

#pragma endregion
};