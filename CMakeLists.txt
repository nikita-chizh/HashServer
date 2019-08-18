cmake_minimum_required(VERSION 3.13)
project(HashServer)

set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(SOURCES
    main.cpp
    TcpServer.h TcpServer.cpp
    Utils.h Utils.cpp
    HashProtocol.h HashProtocol.cpp)

add_executable( HashServer ${SOURCES})
target_link_libraries(HashServer ev)

add_subdirectory(tests)

