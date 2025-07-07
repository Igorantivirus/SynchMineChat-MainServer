#pragma once

#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>

#include "../Services/Service.hpp"

#include "../AsynchContainers/AsynchQueue.hpp"

#include "SubUtility.hpp"
#include "Message.hpp"
#include "ClientSubscriber.hpp"

class ClientBrocker
{
public:
    ClientBrocker()// : processMessagesTh_{&ClientBrocker::processMessages, this}
    {
        //
    }
    ~ClientBrocker()
    {
        stop();
    }

    void subscribe(ClientSubscriber &client)
    {
        std::unique_lock<std::mutex> lg(subsMut_);
        subs_.push_back(client);
    }

    void addMessage(const Message &msg)
    {
        messages_.push(msg);
    }

    void stop()
    {
        Service::log.log("Start to stop brocker.", crow::LogLevel::Info);
        isWorking_ = false;
        messages_.push(Message{}); // Разблокировка ожидания в очереди
        // if (processMessagesTh_.joinable())
        //     processMessagesTh_.join();
        for(auto& i : subs_)
            i.get().stop();
        subs_.clear();
    }

    void run()
    {
        while (isWorking_)
            sendToSubscribers(messages_.pop());
    }

private: // Поля
    // Очередь сообщений (потокобзопасная)
    AsynchSafelyQueue<Message> messages_;
    // std::thread processMessagesTh_;

    // Работа с подписчиками
    std::vector<std::reference_wrapper<ClientSubscriber>> subs_;
    std::mutex subsMut_;

    std::atomic<bool> isWorking_ = true;

private:

    void sendToSubscribers(const Message &msg)
    {
        if (msg.to == ClientType::none)
            return;
        std::unique_lock<std::mutex> lg(subsMut_);
        for (auto &sub : subs_)
        {
            if (sub.get().getID() == msg.fromId)
                continue;
            if (sub.get().getType() == msg.to || msg.to == ClientType::any)
                sub.get().sendMessage(msg);
        }
    }
};