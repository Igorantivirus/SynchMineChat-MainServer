cmake_minimum_required(VERSION 3.16)

include(FetchContent)

project(SynchMineChat-MainServer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# CROW
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Crow REQUIRED)
#end CROW

#nlohmann_json
if(WIN32)

    find_package(nlohmann_json 3.12.0 REQUIRED)
    add_library(nlohmann_json INTERFACE IMPORTED)

else()

    find_package(nlohmann_json REQUIRED)

endif()
#end nlohmann_json

#TgBot
if(WIN32)

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

else()

    find_package(Threads REQUIRED)
    find_package(OpenSSL REQUIRED)
    find_package(Boost COMPONENTS system filesystem REQUIRED)
    include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
    if (CURL_FOUND)
        include_directories(${CURL_INCLUDE_DIRS})
        add_definitions(-DHAVE_CURL)
    endif()


endif()
#end TgBot

add_executable(main src/main.cpp)

target_link_libraries(main
    PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
    ZLIB::ZLIB
    Crow::Crow
    nlohmann_json::nlohmann_json
)

#TgBot
if(WIN32)
    
    target_link_libraries(main PRIVATE TgBot::TgBot)

else()

    target_link_libraries(main PRIVATE /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})

endif()