cmake_minimum_required(VERSION 3.14)
project(VanKampen)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER clang++)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(vankamp main.cpp)
target_link_libraries(vankamp PUBLIC gvc cgraph)