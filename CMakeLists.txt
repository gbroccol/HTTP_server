cmake_minimum_required(VERSION 3.16.5)
project(clion_test)

#set(CMAKE_CXX_STANDARD 98)

add_executable(clion_test sources/main.cpp sources/Config.cpp sources/Handler.cpp sources/ParseRequest.cpp sources/Server.cpp sources/Session.cpp sources/Authentication.hpp sources/Authentication.cpp)