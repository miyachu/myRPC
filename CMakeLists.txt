cmake_minimum_required(VERSION 3.10)
project(RPC)

set(CMAKE_CXX_STANDARD 17)

add_executable(RPC main.cpp server/server.cpp client/client.cpp client/client.cpp client/client.cpp server/server.cpp main.cpp utils/socktools.cpp server/main.cpp client/main.cpp)
