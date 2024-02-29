cmake_minimum_required(VERSION 3.12)
project(circularBuffer)

set(CMAKE_SYSTEM_NAME Linux)

set(TOOLCHAIN_PATH /usr/local/gcc-13.2.0)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/bin/gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/bin/g++)
set(CMAKE_CXX_STANDARD 20)

set(cd ..
    CMAKE_FIND_ROOT_PATH
    ${TOOLCHAIN_PATH}
    "/usr/local/include"  # boost
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

include_directories(include)

set(SOURCES
    source/main.cpp
)
add_executable(circularBuffer ${SOURCES})

# 定义用户可配置的选项
option(ENABLE_DEBUG "Enable debug output" OFF)

if(ENABLE_DEBUG)
  add_definitions(-DDEBUG_OUTPUT)
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)