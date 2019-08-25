cmake_minimum_required(VERSION 3.0)
project(HashServer)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

include_directories(/usr/include)
link_directories(/usr/lib)


set(SOURCES
    TcpServer.h TcpServer.cpp
    Utils.h Utils.cpp
    HashProtocol.h HashProtocol.cpp
    Config.h Config.cpp
    CryptoFunctions.h CryptoFunctions.cpp
    Worker.h
    ThreadSaveQueue.h
    ThreadPool.h)

add_library(ServerLib ${SOURCES})

add_executable( HashServer main.cpp)
target_link_libraries(HashServer ServerLib ev cryptopp pthread)

add_subdirectory(tests)

