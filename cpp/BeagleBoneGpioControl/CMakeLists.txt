cmake_minimum_required(VERSION 3.22.1)

project(BeagleBoneGpioControl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

find_package(GTest REQUIRED)

include_directories(include)

add_subdirectory(src)

add_subdirectory(tests)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE gpio ultrasonic)