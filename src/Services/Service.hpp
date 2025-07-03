#pragma once

#include "Logger.hpp"
#include "Configs/ServerConfig.hpp"

class Service
{
public:
    static ServerConfig config;
    static ServerLogger log;

    static void staticConstruct(const std::string &configFileName)
    {
        config = loadServerConfig(configFileName);
#ifdef _DEBUG
        log.setEntryToConsole(true);
#endif
        log.openFile(config.LOG_FILE);
        crow::logger::setHandler(&log);
    }
};

ServerConfig Service::config;
ServerLogger Service::log;