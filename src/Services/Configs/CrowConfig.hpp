#pragma once

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

struct CrowConfig
{
    int SERVER_PORT{};

    std::string SECRET_KEY{};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(CrowConfig, SERVER_PORT, SECRET_KEY)
};

inline CrowConfig loadCrowConfig(const std::string &config_path)
{
    std::ifstream config_file(config_path);
    auto config = nlohmann::json::parse(config_file);
    config_file.close();
    return config.template get<CrowConfig>();
}