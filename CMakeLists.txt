cmake_minimum_required(VERSION 3.20)
project(deltarune_cnpatcher LANGUAGES CXX)

# ------------------------------
# 1. 设置 C++20 标准
# ------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "C:/external/cmake" ${CMAKE_PREFIX_PATH})

find_package(SDL3 CONFIG REQUIRED COMPONENTS SDL3-shared)
find_package(SDL3_image CONFIG REQUIRED COMPONENTS SDL3_image-shared)
find_package(SDL3_ttf CONFIG REQUIRED COMPONENTS SDL3_ttf-shared)
find_package(SDL3_mixer CONFIG REQUIRED COMPONENTS SDL3_mixer-shared)
find_package(nlohmann_json CONFIG REQUIRED)

# ------------------------------
# 3. 收集源文件
# ------------------------------
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Windows 资源文件
if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND SOURCES "app/win.rc")
endif ()

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
        SDL3::SDL3-shared
        SDL3_image::SDL3_image-shared
        SDL3_ttf::SDL3_ttf-shared
        SDL3_mixer::SDL3_mixer-shared
)

# ------------------------------
# 6. Windows 特定设置
# ------------------------------
if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(deltarune_cnpatcher PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()

# ------------------------------
# 7. 构建后拷贝资源
# ------------------------------
add_custom_command(TARGET deltarune_cnpatcher POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/asset"
        "$<TARGET_FILE_DIR:deltarune_cnpatcher>"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:deltarune_cnpatcher>
            $<TARGET_FILE_DIR:deltarune_cnpatcher>
        COMMAND_EXPAND_LISTS
)
