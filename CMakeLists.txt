cmake_minimum_required(VERSION 3.16)

project(SynchMineChat-MainServer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# CROW
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CROW REQUIRED)

#end CROW

#nlohmann_json
find_package(nlohmann_json 3.12.0 REQUIRED)

add_library(nlohmann_json INTERFACE IMPORTED)
#end nlohmann_json

#TgBot
find_path(TGBOT_INCLUDE_DIR NAMES "tgbot/tgbot.h" PATHS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
find_library(TGBOT_LIBRARY NAMES "TgBot" PATHS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib")

if (NOT TGBOT_INCLUDE_DIR)
    message(FATAL_ERROR "Не удалось найти заголовочные файлы TgBot!")
endif()

if (NOT TGBOT_LIBRARY)
    message(FATAL_ERROR "Не удалось найти библиотеку TgBot!")
endif()

add_library(TgBot::TgBot INTERFACE IMPORTED)
target_include_directories(TgBot::TgBot INTERFACE ${TGBOT_INCLUDE_DIR})
target_link_libraries(TgBot::TgBot INTERFACE ${TGBOT_LIBRARY})
#end TgBot

add_executable(main src/main.cpp)

# CROW
target_link_libraries(main
    PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
    ZLIB::ZLIB
    Crow::Crow

    TgBot::TgBot
    nlohmann_json::nlohmann_json
)