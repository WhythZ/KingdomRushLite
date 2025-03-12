cmake_minimum_required(VERSION 3.20)
project(KingdomRushLite LANGUAGES C CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# 检测是否使用 Emscripten
if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    message(STATUS "Compiling using Emscripten")
    set(USE_EMSCRIPTEN TRUE)
else()
    set(USE_EMSCRIPTEN FALSE)
endif()

# 包含第三方库头文件目录
include_directories(
    Dependencies/cJSON/include
)

# 构建cJSON为静态库
add_library(cJSON STATIC Dependencies/cJSON/cJSON.c)
target_include_directories(cJSON PUBLIC Dependencies/cJSON/include)

# 添加子目录
add_subdirectory(KingdomRushLite)