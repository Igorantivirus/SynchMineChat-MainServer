#pragma once

#include <map>
#include <string>

#include <nlohmann/json.hpp>

#include "SubUtility.hpp"

using RowJson = std::map<std::string, std::string>;

struct Message
{
    ClientType from = ClientType::none;
    ClientType to = ClientType::none;

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
        from = fromString(json["from"].get<std::string>());
        to = fromString(json["to"].get<std::string>());

        for (auto it = json.begin(); it != json.end(); ++it)
        {
            const std::string &key = it.key();
            if (key != "from" && key != "to")
                msg[key] = it.value();
        }
    }

    nlohmann::json toJson() const
    {
        nlohmann::json res;
        res["from"] = toString(from);
        res["to"] = toString(to);
        for (const auto &[key, value] : msg)
            res[key] = value;
        return res;
    }

#pragma endregion
};