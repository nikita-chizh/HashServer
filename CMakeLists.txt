cmake_minimum_required(VERSION 3.13)
project(HashServer)

set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(SOURCES

    TcpServer.h TcpServer.cpp
    Utils.h Utils.cpp
    HashProtocol.h HashProtocol.cpp
    Config.h Config.cpp
    CryptoFunctions.h CryptoFunctions.cpp)

add_library(ServerLib ${SOURCES})

add_executable( HashServer main.cpp)
target_link_libraries(HashServer ev cryptopp ServerLib)

add_subdirectory(tests)

