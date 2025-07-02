#pragma once

#include "Logger.hpp"
#include "ServerConfig.hpp"

class Service
{
public:
    static ServerConfig config;
    static ServerLogger log;

    static void staticConstruct(const std::string &configFileName)
    {
        config = loadConfig(configFileName);
        log.openFile(config.LOG_FILE);
#ifdef _DEBUG
        log.setEntryToConsole(true);
#endif
        crow::logger::setHandler(&log);
    }
};