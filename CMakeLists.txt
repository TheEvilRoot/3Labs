cmake_minimum_required(VERSION 3.0)
project(3Lab5)

set(CMAKE_CXX_STANDARD 17)

add_executable(3Lab5 main.cpp api.h railway/train/train.h railway/engine/engine.cpp railway/engine/engine.h railway/driver/driver.cpp railway/driver/driver.h railway/conductor/conductor.cpp railway/conductor/conductor.h railway/vehicle/vehicle.cpp railway/vehicle/vehicle.h railway/railwaycarriage/railwaycarriage.cpp railway/railwaycarriage/railwaycarriage.h railway/locomotive/locomotive.cpp railway/locomotive/locomotive.h railway/train/train.cpp)