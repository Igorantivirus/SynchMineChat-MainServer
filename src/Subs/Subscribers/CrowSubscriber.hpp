#pragma once

#include <cstdint>
#include <atomic>
#include <vector>
#include <functional>

#include "crow.h"

#include "../../AsynchContainers/AsynchMap.hpp"

#include "../../Services/Service.hpp"

#include "../ClientSubscriber.hpp"
#include "../ClientBrocker.hpp"

#include "../../StringUtility.hpp"

class CrowSubscriber : public ClientSubscriber
{
public:
    CrowSubscriber(ClientBrocker& brocker) : ClientSubscriber{ClientType::server}, brocker_{brocker}
    {
        brocker_.subscribe(*this);
        init();
    }

    void sendMessage(const Message &msg) override
    {
        for(const auto& [key, value] : users_)
            if(value.registered && value.type == msg.to)
                key->send_text(msg.toJson().dump());
    }

    void asynchRun()
    {
        //TODO
    }

private:
    using UserTypePtr = crow::websocket::connection*;

    struct UserInfo
    {
        bool registered = false;
        ClientType type = ClientType::none;
    };

private:
    // посредник
    ClientBrocker &brocker_;

    // Сервер
    crow::SimpleApp app_;
    //Подключённые юзеры
    AsynchSafelyMap<UserTypePtr, UserInfo> users_;

    std::atomic<bool> isWorking_ = true;

private:

    void init()
    {
        CROW_WEBSOCKET_ROUTE(app_, "/ws")
            .onopen([this](crow::websocket::connection &conn)
                    { this->onOpen(conn); })
            .onclose([this](crow::websocket::connection &conn, const std::string &reason, uint16_t v)
                    { this->onClose(conn, reason, v); })
            .onmessage([this](crow::websocket::connection &conn, const std::string &data, bool is_binary)
                    { this->onMessage(conn, data, is_binary); });
    }

    #pragma region Server methods
    
    void onOpen(crow::websocket::connection &conn)
    {
        Service::log.log("New client connected.", crow::LogLevel::Info);
        users_[&conn] = {false};
    }

    void onClose(crow::websocket::connection &conn, const std::string &reason, uint16_t)
    {
        Service::log.log("Some client disconnected.", crow::LogLevel::Info);
        users_.erase(&conn);
    }

    void onMessage(crow::websocket::connection &conn, const std::string &data, bool is_binary)
    {
        Message msg;
        msg.fromJson(toJson(data));

        if(users_.safelyGet(&conn, {}).registered)
            processMessages(conn, msg);
        else
            registrate(conn, msg);
    }
    
    #pragma endregion

    void disconectUser(UserTypePtr user, const std::string response)
    {
        std::lock_guard lg(mut_);
        users_.erase(user);
        user->close(response);
    }

    void processMessages(crow::websocket::connection &conn, const Message& msg)
    {
        std::lock_guard lg(mut_);

        if(msg.to == ClientType::server)
            return;
        brocker_.addMessage(msg);
    }

    void registrate(crow::websocket::connection &conn, const Message& msg)
    {
        std::lock_guard lg(mut_);
        // TODO
        users_[&conn].registered = true;
    }

private:

    nlohmann::json toJson(const std::string& str)
    {
        try
        {
            return nlohmann::json{str};
        }
        catch(...)
        {
            return nlohmann::json{};
        }
        
    }

};