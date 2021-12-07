cmake_minimum_required(VERSION 3.20)
project(ChikaGossip)

set(CMAKE_CXX_STANDARD 14)

include_directories(laptop_factory_src)

add_executable(client
        laptop_factory_src/Socket.h
        laptop_factory_src/Socket.cpp
        laptop_factory_src/Messages.cpp
        laptop_factory_src/Messages.h)

add_executable(server
        laptop_factory_src/ServerMain.cpp
        laptop_factory_src/ServerBody.h
        laptop_factory_src/ServerBody.cpp
        laptop_factory_src/ServerStub.h
        laptop_factory_src/ServerStub.cpp
        laptop_factory_src/ServerSocket.h
        laptop_factory_src/ServerSocket.cpp
        laptop_factory_src/Socket.h
        laptop_factory_src/Socket.cpp
        laptop_factory_src/Messages.cpp
        laptop_factory_src/Messages.h)