#pragma once

#include <cstdint>
#include <set>
#include <map>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

struct TgBotConfig
{
    std::set<std::int64_t> ADMINS_ID;
    std::set<std::int64_t> CHATS_ID;
    std::map<std::string, std::string> USERS_NICKS;

    const std::string& getMinecraftName(const std::string& tgName) const
    {
        std::cout << "finding: " << tgName << '\n';
        std::cout << "my dict\n";
        for(const auto& [ky, value] : USERS_NICKS)
            std::cout << ky << ' ' << value << '\n';
        const auto found = USERS_NICKS.find(tgName);
        if(found == USERS_NICKS.end())
            std::cout << "not found\n";
        else
            std::cout << "found\n";
        return found == USERS_NICKS.end() ? tgName : found->second;
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(TgBotConfig, ADMINS_ID, CHATS_ID, USERS_NICKS)
};

inline TgBotConfig loadTgBotConfig(const std::string &config_path)
{
    std::ifstream config_file(config_path);
    auto config = nlohmann::json::parse(config_file);
    config_file.close();
    std::cout << "i read: " << config.dump() << '\n';
    return config.template get<TgBotConfig>();
}

inline void saveTgBotConfig(const TgBotConfig& conf, const std::string &fileName)
{
    nlohmann::json json;
    json["ADMINS_ID"] = conf.ADMINS_ID;
    json["CHATS_ID"] = conf.CHATS_ID;
    json["USERS_NICKS"] = conf.USERS_NICKS;
    std::ofstream configFile(fileName);
    configFile << json;
    configFile.close();
}