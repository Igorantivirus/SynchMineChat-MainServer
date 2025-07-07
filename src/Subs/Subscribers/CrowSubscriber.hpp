#pragma once

#include <cstdint>
#include <atomic>
#include <vector>
#include <functional>

#include "crow.h"

#include "../../AsynchContainers/AsynchMap.hpp"

#include "../../Services/Configs/CrowConfig.hpp"
#include "../../Services/Service.hpp"

#include "../ClientSubscriber.hpp"
#include "../ClientBrocker.hpp"

#include "../../StringUtility.hpp"

class CrowSubscriber : public ClientSubscriber
{
public:
    CrowSubscriber(ClientBrocker& brocker) : ClientSubscriber{ClientType::server}, brocker_{brocker}, config_{loadCrowConfig(Service::config.CROW_CONFIG)}
    {
        brocker_.subscribe(*this);
        init();
    }

    void sendMessage(const Message &msg) override
    {
        std::string textToSend = msg.toJson().dump();
        Service::log.log("Message to minecraft!", crow::LogLevel::Info);
        Service::log.log("Next send: " + textToSend, crow::LogLevel::Debug);
        for(const auto& [key, value] : users_)
            if(value.registered && (value.type == msg.to || msg.to == ClientType::any))
                key->send_text(textToSend);
    }

    void stop() override
    {
        Service::log.log("Crow start to stopp.", crow::LogLevel::Info);
        app_.port(config_.SERVER_PORT).multithreaded().stop();
        server_future_.wait();
        Service::log.log("Crow stopped.", crow::LogLevel::Info);
    }

    void multithreadedRun()
    {
        server_future_ = app_.port(config_.SERVER_PORT).multithreaded().run_async();
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

    CrowConfig config_;

    std::shared_future<void> server_future_;

private:

    void init()
    {
        CROW_WEBSOCKET_ROUTE(app_, "/ws")
            .onopen([this](crow::websocket::connection &conn)
                    { this->onOpen(conn); })
            .onmessage([this](crow::websocket::connection &conn, const std::string &data, bool is_binary)
                       { this->onMessage(conn, data, is_binary); })
        #if defined(_WIN32) || defined(_WIN64)
            .onclose([this](crow::websocket::connection &conn, const std::string &reason, uint16_t)
                     { this->onClose(conn, reason); });
        #else
            .onclose([this](crow::websocket::connection &conn, const std::string &reason)
                            { this->onClose(conn, reason); });
        #endif
    }

    #pragma region Server methods
    
    void onOpen(crow::websocket::connection &conn)
    {
        Service::log.log("New client connected.", crow::LogLevel::Info);
        users_[&conn] = {false};
    }

    void onClose(crow::websocket::connection &conn, const std::string &reason)
    {
        Service::log.log("Some client disconnected.", crow::LogLevel::Info);
        users_.erase(&conn);
    }

    void onMessage(crow::websocket::connection &conn, const std::string &data, bool is_binary)
    {
        Message msg;
        msg.fromJson(toJson(data));
        msg.fromId = id_;

        UserInfo pass;
        if(users_.safelyGet(&conn, pass).registered)
            processMessages(conn, msg);
        else
            registrate(conn, msg);
    }
    
    #pragma endregion

    void disconectUser(UserTypePtr user, const std::string response)
    {
        users_.erase(user);
        user->close(response);
    }

    void processMessages(crow::websocket::connection &conn, const Message& msg)
    {
        if(msg.to == ClientType::server)
            return;
        brocker_.addMessage(msg);
    }

    void registrate(crow::websocket::connection &conn, const Message& msg)
    {
        if(msg["key"] != config_.SECRET_KEY)
            disconectUser(&conn, "Failed regiostration");
        users_[&conn].registered = true;
    }

private:

    static nlohmann::json toJson(const std::string& str)
    {
        try
        {
            return nlohmann::json::parse(str);
        }
        catch(...)
        {
            return nlohmann::json{};
        }
        
    }

};