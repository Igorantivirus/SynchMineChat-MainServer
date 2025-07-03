#pragma once

#include <string>
#include <map>

using IDType = unsigned short;

enum class ClientType : unsigned char
{
    none = 0,
    telegram,
    minecraft,
    server,
    any
};

std::string toString(const ClientType f)
{
    // clang-format off
    const static std::map<ClientType, std::string> converter =
    {
        { ClientType::none,       "none"      },
        { ClientType::telegram,   "tg"        },
        { ClientType::minecraft,  "mine"      },
        { ClientType::server,     "server"    },
        { ClientType::any,        "any"       }
    };
    // clang-format on
    return converter.find(f)->second;
}
ClientType toClientType(const std::string &str)
{
    // clang-format off
    const static std::map<std::string, ClientType> converter =
    {
        { "tg",     ClientType::telegram    },
        { "mine",   ClientType::minecraft   },
        { "server", ClientType::server      },
        { "any",    ClientType::any         }
    };
    // clang-format on
    const auto found = converter.find(str);
    return found == converter.end() ? ClientType::none : found->second;
}

enum class MessageType : unsigned char
{
    none = 0,
    text,
    audio,
    command
};

std::string toString(const MessageType type)
{
    // clang-format off
    std::map<MessageType, std::string> converter =
    {
        {MessageType::none,     "none"      },
        {MessageType::text,     "text"      },
        {MessageType::audio,    "audio"     },
        {MessageType::command,  "command"   }
    };
    // clang-format on
    return converter.find(type)->second;
}

MessageType toMessageType(const std::string type)
{
    // clang-format off
    std::map<std::string, MessageType> converter =
    {
        {"none",    MessageType::none   },
        {"text",    MessageType::text   },
        {"audio",   MessageType::audio  },
        {"command", MessageType::command}
    };
    // clang-format on
    const auto found = converter.find(type);
    return found == converter.end() ? MessageType::none : found->second;
}