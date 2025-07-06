#pragma once

#include <map>
#include <string>

#include <nlohmann/json.hpp>

#include "SubUtility.hpp"

using RowJson = std::map<std::string, std::string>;

struct Message
{
    IDType fromId{};

    ClientType from = ClientType::none;
    ClientType to = ClientType::none;

    MessageType type = MessageType::none;

    RowJson msg;

public:
    Message() = default;

#pragma region Converting

    explicit Message(const nlohmann::json &json)
    {
        fromJson(json);
    }

    Message &operator=(const nlohmann::json &json)
    {
        fromJson(json);
        return *this;
    }

    explicit operator nlohmann::json() const
    {
        return toJson();
    }

    void fromJson(const nlohmann::json &json)
    {
        from = toClientType(json.value("from", std::string("none")));
        to = toClientType(json.value("to", std::string("none")));
        type = toMessageType(json.value("type", std::string("none")));
        fromId = json.value("fromID", 0);

        for (auto it = json.begin(); it != json.end(); ++it)
        {
            const std::string &key = it.key();
            if (key != "from" && key != "to" && key != "fromID" && key != "type")
                msg[key] = it.value();
        }
    }

    nlohmann::json toJson() const
    {
        nlohmann::json res;
        res["fromID"] = std::to_string(fromId);
        res["from"] = toString(from);
        res["to"] = toString(to);
        res["type"] = toString(type);
        for (const auto &[key, value] : msg)
            res[key] = value;
        return res;
    }

#pragma endregion

    const std::string operator[](const std::string& key) const
    {
        const auto found = msg.find(key);
        return found == msg.end() ? std::string() : found->second;
    }

};