cmake_minimum_required(VERSION 3.0)
project(3Lab5)

set(CMAKE_CXX_STANDARD 17)

add_executable(3Lab5 main.cpp dev/api.h dev/developer.h dev/webdeveloper.h dev/nativedeveloper.h dev/frontenddeveloper.h dev/serverdeveloper.h dev/fullstackdeveloper.h)