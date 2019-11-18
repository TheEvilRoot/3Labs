cmake_minimum_required(VERSION 3.15)
project(3Lab5)

set(CMAKE_CXX_STANDARD 17)

add_executable(3Lab5 main.cpp api.h railway/train.h railway/engine.cpp railway/engine.h railway/driver.cpp railway/driver.h railway/conductor.cpp railway/conductor.h railway/vehicle.cpp railway/vehicle.h railway/railwaycarriage.cpp railway/railwaycarriage.h railway/locomotive.cpp railway/locomotive.h railway/train.cpp)