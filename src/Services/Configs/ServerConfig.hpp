#pragma once

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

struct ServerConfig
{
    std::string LOG_FILE;
    std::string TG_BOT_KEY;
    std::string TG_BOT_CONFIG;
    std::string TG_BOT_RESPONSE_CONFIG;
    std::string CROW_CONFIG;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(ServerConfig, LOG_FILE, TG_BOT_KEY, TG_BOT_CONFIG, TG_BOT_RESPONSE_CONFIG, CROW_CONFIG)
};

inline ServerConfig loadServerConfig(const std::string &config_path)
{
    std::ifstream config_file(config_path);
    auto config = nlohmann::json::parse(config_file);
    config_file.close();
    return config.template get<ServerConfig>();
}