#include <iostream>
#include <string>

#include "Services/Service.hpp"

#include "Subs/ClientBrocker.hpp"

#include "Services/Configs/TgBotConfig.hpp"

#include "StringUtility.hpp"

#include "Subs/Subscribers/TgBotSubscriber.hpp"

int main()
{
    Service::staticConstruct("resources\\ServerConfig.json");

    Service::log.log(to_utf8(L"Привет, АБОБА"), crow::LogLevel::Info);

    // TgBotConfig conf = loadTgBotConfig("resources\\TgBotConfig.json");
    // // conf.loadTgBotConfig();

    // // TgBotConfig conf = loadTgBotConfig("resources\\TgBotConfig.json");

    // for (const auto &i : conf.ADMINS_ID)
    //     Service::log.log(std::to_string(i), crow::LogLevel::Info);

    // for (const auto &i : conf.CHATS_ID)
    //     Service::log.log(std::to_string(i), crow::LogLevel::Info);

    // saveTgBotConfig(conf, "resources\\TgBotConfig.json");

    return 0;
}