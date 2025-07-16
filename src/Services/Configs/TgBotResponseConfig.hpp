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
    std::string admin_help_command;
    std::string online_comand_success;
    std::string online_comand_fatal;
    std::string renew_command_success;
    std::string renew_command_already;
    std::string renew_command_fatal;
    std::string update;
    std::string start_chat_command;
    std::string stop_chat_command;
    std::string not_admin;
    std::string left_the_game;
    std::string join_the_game;
    std::string achievement;
    std::string stoping_run;
    std::string bind_succes;
    std::string command_params_error;
    std::string delete_succes;
    std::string delete_fatal;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(TgBotResponseConfig, start_command, help_comand, admin_help_command, online_comand_success, online_comand_fatal, renew_command_success, renew_command_already, renew_command_fatal, update, start_chat_command, stop_chat_command, not_admin, left_the_game, join_the_game, achievement, stoping_run, bind_succes, command_params_error, delete_succes, delete_fatal)
};

inline TgBotResponseConfig loadTgBotResponseConfig(const std::string &config_path)
{
    std::ifstream config_file(config_path);
    auto config = nlohmann::json::parse(config_file);
    config_file.close();
    return config.template get<TgBotResponseConfig>();
}