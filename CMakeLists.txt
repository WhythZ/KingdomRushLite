cmake_minimum_required(VERSION 3.20)
project(KingdomRushLite LANGUAGES C CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# 包含第三方库头文件目录
include_directories(
    Dependencies/cJSON/include
    Dependencies/SDL2/include
    Dependencies/SDL2_gfx/include
    Dependencies/SDL2_image/include
    Dependencies/SDL2_mixer/include
    Dependencies/SDL2_ttf/include
)

# 链接库目录
link_directories(
    Dependencies/SDL2/x64lib
    Dependencies/SDL2_gfx/x64lib
    Dependencies/SDL2_image/x64lib
    Dependencies/SDL2_mixer/x64lib
    Dependencies/SDL2_ttf/x64lib
)

# 构建cJSON为静态库
add_library(cJSON STATIC Dependencies/cJSON/cJSON.c)
target_include_directories(cJSON PUBLIC Dependencies/cJSON/include)

# 添加子目录
add_subdirectory(KingdomRushLite)