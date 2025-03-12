# 递归收集所有源文件
file(GLOB_RECURSE SOURCES 
    "Scripts/Source/*.cpp"
    "Scripts/Source/**/*.cpp"
)

# 创建可执行文件
add_executable(KingdomRushLite ${SOURCES})

# Emscripten相关配置
if (USE_EMSCRIPTEN)
    # 设置编译目标文件为HTML
    set_target_properties(KingdomRushLite PROPERTIES SUFFIX ".html")

    # 在编译时使用Emscripten内置的SDL2相关库，无需添加.dll文件
    target_compile_options(KingdomRushLite PRIVATE
        "-sUSE_SDL=2"              # 使用Emscripten提供的 SDL2
        "-sUSE_SDL_IMAGE=2"        # 启用SDL2_image
        "-sUSE_SDL_MIXER=2"        # 启用SDL2_mixer
        "-sUSE_SDL_TTF=2"          # 启用SDL2_ttf
        "-sUSE_SDL_GFX=2"          # 启用SDL2_gfx
    )
    
    # # 如果代码中有使用SDL_GetTicks64，需确保启用SDL2 2.0.18+ 特性
    # target_compile_options(KingdomRushLite PRIVATE
    #     "-sSDL2_IMAGE_FORMATS='[\"png\"]'"  # 按需指定图像格式
    # )

    # 链接cJSON库
    target_link_libraries(KingdomRushLite PRIVATE cJSON)

    # 显示声明链接SDL相关库
    target_link_libraries(KingdomRushLite PRIVATE
        "-sUSE_SDL=2"
        "-sUSE_SDL_IMAGE=2"
        "-sUSE_SDL_MIXER=2"
        "-sUSE_SDL_TTF=2"
        "-sUSE_SDL_GFX=2"
    )

    # 这里用target_link_options传递Emscripten的链接参数，将资源和配置文件加载到虚拟文件系统
    target_link_options(KingdomRushLite PRIVATE
        "-sFORCE_FILESYSTEM"  # 允许文件系统操作
        "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/KingdomRushLite/Assets@/Assets"
        "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/KingdomRushLite/Data@/Data"
    )
endif()