#pragma once

#include <string>
#include <mutex>

#include "SubUtility.hpp"
#include "Message.hpp"

class ClientSubscriber
{
public:
    ClientSubscriber(const ClientType type) : type_{type}, id_{++countSubers_}
    {
    }
    virtual ~ClientSubscriber() = default;

    virtual void sendMessage(const Message& msg) = 0;

    const ClientType getType() const
    {
        return type_;
    }
    const IDType getID() const
    {
        return id_;
    }

protected:
    mutable std::mutex mut_;
    const ClientType type_ = ClientType::none;
    const IDType id_;

protected:
    static IDType countSubers_;
};

IDType ClientSubscriber::countSubers_ = 0;