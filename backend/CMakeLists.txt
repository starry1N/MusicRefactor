cmake_minimum_required(VERSION 3.16)
project(MusicFreeBackend VERSION 0.1.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ============================================================
# 依赖库配置
# ============================================================

# 查找依赖
find_package(Threads REQUIRED)

# 可选：FFmpeg 支持（音频处理）
# find_package(FFmpeg REQUIRED)

# 可选：SQLite3 支持（数据库）
# find_package(SQLite3 REQUIRED)

# ============================================================
# 源文件
# ============================================================

# 核心库源文件
set(CORE_SOURCES
    src/core/audio_engine.cpp
    src/core/playlist_manager.cpp
)

# 网络服务源文件
set(NETWORK_SOURCES
    src/network/api_server.cpp
)

# 数据库源文件
set(DATABASE_SOURCES
    # src/database/database_manager.cpp
)

# 插件系统源文件
set(PLUGIN_SOURCES
    # src/plugin/plugin_manager.cpp
)

# 所有源文件
set(ALL_SOURCES
    ${CORE_SOURCES}
    ${NETWORK_SOURCES}
    ${DATABASE_SOURCES}
    ${PLUGIN_SOURCES}
)

# ============================================================
# 编译目标
# ============================================================

# 核心库（静态库）
add_library(musicfree_core STATIC ${CORE_SOURCES})
target_include_directories(musicfree_core PUBLIC include)
target_link_libraries(musicfree_core PRIVATE Threads::Threads)

# 网络服务库（静态库）
add_library(musicfree_network STATIC ${NETWORK_SOURCES})
target_include_directories(musicfree_network PUBLIC include)
target_link_libraries(musicfree_network PRIVATE musicfree_core Threads::Threads)

# 主应用（控制台）
add_executable(musicfree_server src/main.cpp)
target_include_directories(musicfree_server PRIVATE include)
target_link_libraries(musicfree_server PRIVATE musicfree_core musicfree_network Threads::Threads)

# ============================================================
# 编译选项
# ============================================================

if(MSVC)
    # Windows MSVC 编译器
    target_compile_options(musicfree_core PRIVATE /W4)
    target_compile_options(musicfree_network PRIVATE /W4)
    target_compile_options(musicfree_server PRIVATE /W4)
else()
    # GCC/Clang 编译器
    target_compile_options(musicfree_core PRIVATE -Wall -Wextra -Werror -fPIC)
    target_compile_options(musicfree_network PRIVATE -Wall -Wextra -Werror -fPIC)
    target_compile_options(musicfree_server PRIVATE -Wall -Wextra)
endif()

# ============================================================
# 测试（可选）
# ============================================================

enable_testing()

# 可以在此处添加测试目标
# add_subdirectory(tests)

# ============================================================
# 安装规则
# ============================================================

install(TARGETS musicfree_server
        RUNTIME DESTINATION bin)

install(TARGETS musicfree_core musicfree_network
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(FILES include/audio_engine.h
              include/api_server.h
              include/playlist_manager.h
              include/database_manager.h
              include/plugin_manager.h
        DESTINATION include/musicfree)

# ============================================================
# 消息输出
# ============================================================

message(STATUS "========================================")
message(STATUS "MusicFree Backend Build Configuration")
message(STATUS "========================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Binary directory: ${CMAKE_BINARY_DIR}")
message(STATUS "========================================")
