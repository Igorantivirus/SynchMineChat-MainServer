cmake_minimum_required(VERSION 3.16)

project(SynchMineChat-MainServer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# CROW
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CROW REQUIRED)

add_executable(main src/main.cpp)

# CROW
target_link_libraries(main
    PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
    ZLIB::ZLIB
    Crow::Crow
)