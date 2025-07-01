#pragma once

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Config
{
    std::string LOG_FILE;

    std::string TG_BOT_KEY;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Config, LOG_FILE, TG_BOT_KEY)
};

inline Config loadConfig(const std::string &config_path)
{
    std::ifstream config_file(config_path);
    auto config = json::parse(config_file);
    config_file.close();
    return config.template get<Config>();
}