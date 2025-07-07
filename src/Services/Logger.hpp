#pragma once

#include <fstream>
#include <string>
#include <mutex>
#include <map>
#include <iostream>
#include <utility>
#include <chrono>
#include <format>

#include "crow.h"

class ServerLogger : public crow::ILogHandler
{
public:
    ServerLogger() = default;
    ServerLogger(const std::string &fileName, const bool writeToConsole = false) : writeToConsole_{writeToConsole}
    {
        openFile(fileName);
    }
    ~ServerLogger()
    {
        closeFile();
    }

    bool isEntryToConsole() const
    {
        return writeToConsole_;
    }
    void setEntryToConsole(const bool writeToConsole)
    {
        std::lock_guard<std::mutex> lg(mut_);
        writeToConsole_ = writeToConsole;
    }

    void closeFile()
    {
        log("---  Log session ended  ---", crow::LogLevel::Info);
        std::lock_guard<std::mutex> lg(mut_);
        logFile_.close();
    }
    bool openFile(const std::string &fileName)
    {
        if (isOpen())
            closeFile();
        mut_.lock();
        logFile_.open(fileName, std::ios::app);
        mut_.unlock();
            
        log("--- Log session started ---", crow::LogLevel::Info);
        
        if(!logFile_.is_open())
           log("Couldn't open the logs file: " + fileName, crow::LogLevel::Warning);
        
        return logFile_.is_open();
    }
    bool isOpen() const
    {
        std::lock_guard<std::mutex> lg(mut_);
        return logFile_.is_open();
    }

    void log(std::string message, crow::LogLevel level) override
    {
        std::lock_guard<std::mutex> lg(mut_);

        std::string time = getCurentTime();

        const auto [strName, color] = toString(level);

        std::string resultMessage = time + ' ' + strName + ' ' + message + '\n';

        if (logFile_.is_open())
            logFile_ << resultMessage;
        if (writeToConsole_)
        {
            setColor(color);
            std::cerr << resultMessage;
            setColor(Color::Default);
        }
    }

private:
    std::ofstream logFile_;
    mutable std::mutex mut_;
    bool writeToConsole_ = false;

private: // static
    enum class Color : unsigned char
    {
        Black = 30, // DEBUG
        Red,        // ERROR
        Green,      // INFO
        Yellow,     // WARNING
        Blue,
        Purple, // CRITICAL
        Cyan,
        White,
        Default = 39 // DEBUG
    };

    static void setColor(const Color c)
    {
        std::cerr << '\x1b' << '[' << static_cast<int>(c) << 'm';
    }

    static std::pair<std::string, Color> toString(const crow::LogLevel level)
    {
        // clang-format off
        switch (level)
        {
            case crow::LogLevel::Critical:  return {"[  FATAL  ]", Color::Purple  };
            case crow::LogLevel::Debug:     return {"[  DEBUG  ]", Color::Default };
            case crow::LogLevel::Error:     return {"[  ERROR  ]", Color::Red     };
            case crow::LogLevel::Info:      return {"[   INF   ]", Color::Green   };
            case crow::LogLevel::Warning:   return {"[ WARNING ]", Color::Yellow  };
            default:                        return {"[ UNKNOWN ]", Color::Default };
        }
        // clang-format on

        // // clang-format off
        // static const std::map<crow::LogLevel, std::pair<std::string, Color>> converter =
        // {
        //     {crow::LogLevel::Critical,  {"[ CRITICAL ]", Color::Purple  }},
        //     {crow::LogLevel::Debug,     {"[ DEBUG    ]", Color::Default }},
        //     {crow::LogLevel::Error,     {"[ ERROR    ]", Color::Red     }},
        //     {crow::LogLevel::Info,      {"[ INFO     ]", Color::Green   }},
        //     {crow::LogLevel::Warning,   {"[ WARNING  ]", Color::Yellow  }}
        // };
        // // clang-format on
        // std::cout << converter.empty() << '\n';
        // return converter.find(level)->second;
    }

    static std::string getCurentTime()
    {
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch()) %
                  1000;
        return std::format("({:%d-%m-%Y %X}:{:03d})", now, ms.count());
    }
};
