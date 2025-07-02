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
ClientType fromString(const std::string &str)
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