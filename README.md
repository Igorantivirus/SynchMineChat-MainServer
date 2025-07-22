# SynchMineChat-MainServer
## (Synchronous Minecraft Chat — Main Server) 

[![English version](https://img.shields.io/badge/English%20version-blue)](README.md)

## Description

A Telegram bot for managing a synchronous Minecraft chat.
Sends messages from a Telegram chat to a Minecraft parser.

Capable of handling:
1) Text messages
2) Voice messages (in development)
3) Media files

The Minecraft server can be found here: [Game Server Link](https://github.com/Igorantivirus/SynchMineChat-GameServer)

## Dependencies

Programming Language: **C++20**

Библиотеки:
* [tgbot-cpp](https://github.com/reo7sp/tgbot-cpp) – Telegram Bot C+
* [CROW]([https://www.boost.org/](https://github.com/CrowCpp/Crow)) – Web Server
* [nlohmann-json](https://github.com/nlohmann/json) – JSON library

## Implementation Features

* Runs in multithreaded mode
  * Main thread
  * Separate threads for CROW and TgBot
  * Additional threads for CROW (internal implementation)
* JSON configuration file support

## Installation and Running

### Prerequisites
* C++20-compatible compiler (GCC 10+, Clang 10+, MSVC 19.29+)
* CMake 3.15+
* Installed dependencies (see above)

### Build Instructions

1. **Clone the repository**
   ```sh
   git clone https://github.com/Igorantivirus/SynchMineChat-MainServer
   cd SynchMineChat-MainServer 
   ```
2. **Configure CMake**
   ```sh
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   ```
3. **Build the project** 
   ```sh
   cmake --build build --config Release
   ```

### Running 

1. Create configuration files (config structure is described in src/Services/Configs):
  * CrowConfig.json
  * ServerConfig.json
  * TgBotConfig.json
2. Navigate to the `build` directory and run the program:
   ```sh
   ./main
   ``` 

## License 
MIT License
