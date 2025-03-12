# 递归收集所有源文件
file(GLOB_RECURSE SOURCES 
    "Scripts/Source/*.cpp"
    "Scripts/Source/**/*.cpp"
)

# 创建可执行文件
add_executable(KingdomRushLite ${SOURCES})

# 链接cJSON库
target_link_libraries(KingdomRushLite PRIVATE cJSON)

# Emscripten相关配置
if (USE_EMSCRIPTEN)
    # 设置编译目标文件为HTML
    set_target_properties(KingdomRushLite PROPERTIES SUFFIX ".html")

    # 使用Emscripten内置的SDL2相关库，而不是使用本地原本的SDL2库，且无需添加.dll文件
    target_compile_options(KingdomRushLite PRIVATE
        "-sUSE_SDL=2"              # 使用Emscripten提供的 SDL2
        "-sUSE_SDL_IMAGE=2"        # 启用SDL2_image
        "-sUSE_SDL_MIXER=2"        # 启用SDL2_mixer
        "-sUSE_SDL_TTF=2"          # 启用SDL2_ttf
        "-sUSE_SDL_GFX=2"          # 启用SDL2_gfx
    )

    # 这里用target_link_options传递Emscripten的链接参数，将资源和配置文件加载到虚拟文件系统
    target_link_options(KingdomRushLite PRIVATE
        "-sFORCE_FILESYSTEM"  # 允许文件系统操作
        "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/KingdomRushLite/Assets@/Assets"
        "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/KingdomRushLite/Data@/Data"
    )
endif()