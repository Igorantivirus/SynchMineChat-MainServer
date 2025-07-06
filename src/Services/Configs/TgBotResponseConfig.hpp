#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>
// "commands":                 ["/start", "/help", "/online", "/renew", "/update", "/registrate"],
struct TgBotResponseConfig
{
    std::string start_command;
    std::string help_comand;
    std::string online_comand_success;
    std::string online_comand_fatal;
    std::string renew_command_success;
    std::string renew_command_already;
    std::string renew_command_fatal;
    std::string update;
    std::string registrate_command;
    std::string not_admin;
    std::string left_the_game;
    std::string join_the_game;
    std::string achievement;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(TgBotResponseConfig, start_command, help_comand, online_comand_success, online_comand_fatal, renew_command_success, renew_command_already, renew_command_fatal, update, registrate_command, not_admin, left_the_game, join_the_game, achievement)
};

inline TgBotResponseConfig loadTgBotResponseConfig(const std::string &config_path)
{
    std::ifstream config_file(config_path);
    auto config = nlohmann::json::parse(config_file);
    config_file.close();
    return config.template get<TgBotResponseConfig>();
}