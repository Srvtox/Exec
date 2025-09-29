cmake_minimum_required(VERSION 3.10.2)
project("nativeexec")

add_library(nativeexec SHARED
            exec.cpp)

find_library(log-lib log)

target_include_directories(nativeexec PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(nativeexec ${log-lib})
