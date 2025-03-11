# 递归收集所有源文件
file(GLOB_RECURSE SOURCES 
    "Scripts/Source/*.cpp"
    "Scripts/Source/**/*.cpp"
)

# 创建可执行文件
add_executable(KingdomRushLite ${SOURCES})

# 链接依赖库
target_link_libraries(KingdomRushLite
    SDL2
    SDL2main
    SDL2_gfx
    SDL2_image
    SDL2_mixer
    SDL2_ttf
    cJSON
)

# Windows子系统设置（MSVC 和 MinGW 兼容）
if(WIN32)
    if(MSVC)
        set_target_properties(KingdomRushLite PROPERTIES 
            LINK_FLAGS "/SUBSYSTEM:WINDOWS"
        )
    elseif(MINGW)
        target_link_libraries(KingdomRushLite -mwindows)
    endif()
endif()
# if(WIN32)
#     set_target_properties(KingdomRushLite PROPERTIES 
#         LINK_FLAGS "/SUBSYSTEM:WINDOWS"
#     )
# endif()

# 复制资源文件到输出目录
add_custom_command(TARGET KingdomRushLite POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
        "$<TARGET_FILE_DIR:KingdomRushLite>/Assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Data"
        "$<TARGET_FILE_DIR:KingdomRushLite>/Data"
)

# 复制主DLL文件
set(MAIN_DLLS
    SDL2.dll
    SDL2_gfx.dll
    SDL2_image.dll
    SDL2_mixer.dll
    SDL2_ttf.dll
)

foreach(DLL ${MAIN_DLLS})
    add_custom_command(TARGET KingdomRushLite POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/${DLL}"
            "$<TARGET_FILE_DIR:KingdomRushLite>/"
    )
endforeach()

# 复制可选依赖DLL（SDL2_image）
file(GLOB SDL2_IMAGE_OPTIONAL_DLLS
    "${CMAKE_SOURCE_DIR}/Dependencies/SDL2_image/x64lib/optional/*.dll"
)
foreach(DLL ${SDL2_IMAGE_OPTIONAL_DLLS})
    add_custom_command(TARGET KingdomRushLite POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${DLL}"
            "$<TARGET_FILE_DIR:KingdomRushLite>/"
    )
endforeach()

# 复制可选依赖DLL（SDL2_mixer）
file(GLOB SDL2_MIXER_OPTIONAL_DLLS
    "${CMAKE_SOURCE_DIR}/Dependencies/SDL2_mixer/x64lib/optional/*.dll"
)
foreach(DLL ${SDL2_MIXER_OPTIONAL_DLLS})
    add_custom_command(TARGET KingdomRushLite POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${DLL}"
            "$<TARGET_FILE_DIR:KingdomRushLite>/"
    )
endforeach()