cmake_minimum_required(VERSION 3.20)
project(deltarune_cnpatcher LANGUAGES CXX)

# ------------------------------
# 1. 设置 C++20 标准
# ------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------
# 2. FetchContent 下载依赖
# ------------------------------
include(FetchContent)

# nlohmann_json
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

# SDL3
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.0
)
FetchContent_MakeAvailable(SDL3)

# SDL3_image
FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG main
)
FetchContent_MakeAvailable(SDL3_image)

# SDL3_ttf
FetchContent_Declare(
        SDL3_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG main
)
FetchContent_MakeAvailable(SDL3_ttf)

# SDL3_mixer
FetchContent_Declare(
        SDL3_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG main
)
FetchContent_MakeAvailable(SDL3_mixer)
# ------------------------------
# 3. 收集源文件
# ------------------------------
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Windows 资源文件
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND SOURCES "app/win.rc")
endif()

add_executable(deltarune_cnpatcher ${SOURCES})

# ------------------------------
# 4. 宏定义
# ------------------------------
target_compile_definitions(deltarune_cnpatcher PRIVATE UNICODE _UNICODE)

# ------------------------------
# 5. 链接库
# ------------------------------
target_link_libraries(deltarune_cnpatcher PRIVATE
        nlohmann_json::nlohmann_json
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
)

# ------------------------------
# 6. Windows 特定设置
# ------------------------------
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    # set_target_properties(deltarune_cnpatcher PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# ------------------------------
# 7. 构建后拷贝资源
# ------------------------------
add_custom_command(TARGET deltarune_cnpatcher POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/external" "$<TARGET_FILE_DIR:deltarune_cnpatcher>/external"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/image" "$<TARGET_FILE_DIR:deltarune_cnpatcher>/image"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/lang" "$<TARGET_FILE_DIR:deltarune_cnpatcher>/lang"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/font" "$<TARGET_FILE_DIR:deltarune_cnpatcher>/font"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/readme.txt" "$<TARGET_FILE_DIR:deltarune_cnpatcher>/" OPTIONAL
)

# 拷贝 dlls
file(GLOB DLL_FILES "${CMAKE_SOURCE_DIR}/dlls/*.dll")
if(DLL_FILES)
    add_custom_command(TARGET deltarune_cnpatcher POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL_FILES} "$<TARGET_FILE_DIR:deltarune_cnpatcher>/"
    )
endif()
