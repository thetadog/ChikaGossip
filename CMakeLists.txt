cmake_minimum_required(VERSION 3.20)
project(ChikaGossip)

set(CMAKE_CXX_STANDARD 14)

include_directories(laptop_factory_src)

add_executable(client
        laptop_factory_src/ClientMain.cpp
        laptop_factory_src/ClientSocket.cpp
        laptop_factory_src/ClientSocket.h
        laptop_factory_src/ClientStub.cpp
        laptop_factory_src/ClientStub.h
        laptop_factory_src/ClientBody.cpp
        laptop_factory_src/ClientBody.h
        laptop_factory_src/ClientTimer.cpp
        laptop_factory_src/ClientTimer.h
        laptop_factory_src/Messages.cpp
        laptop_factory_src/Messages.h)
