cmake_minimum_required(VERSION 3.8)
project(HashServer)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

include_directories(/usr/include)
link_directories(/usr/lib)


set(SOURCES
    src/TcpServer.h src/TcpServer.cpp
    src/Utils.h src/Utils.cpp
    src/HashProtocol.h src/HashProtocol.cpp
    src/Config.h src/Config.cpp
    src/CryptoFunctions.h src/CryptoFunctions.cpp
    src/Worker.h
    src/ThreadSaveQueue.h
    src/ThreadPool.h)

add_library(ServerLib ${SOURCES})

add_executable( HashServer src/main.cpp)
target_link_libraries(HashServer ServerLib ev cryptopp pthread)

add_subdirectory(tests)

