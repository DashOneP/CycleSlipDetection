cmake_minimum_required(VERSION 3.16)

# 1. 项目信息
project(CycleSlipDetection LANGUAGES CXX)

# 2. 编译选项设置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

# 3. 收集源文件 (关键：确保所有 .cc 都被包含)
# 显式列出文件是大型工程最稳健的做法
set(PROJECT_SOURCES
    src/main.cc
    src/main_app_logic.cc
    src/read_rnx_file.cc
    src/global_variable.cc

)

# 4. 生成可执行程序
add_executable(CycleSlipDetection ${PROJECT_SOURCES})

# 5. 设置头文件搜索路径
# PRIVATE 表示这些头文件路径仅用于本项目编译
target_include_directories(CycleSlipDetection PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src  # 如果 src 内部还有 .h，保留这一行
)

# 6. 查找并链接线程库 (GNSS处理通常涉及多线程)
find_package(Threads REQUIRED)
find_package(absl REQUIRED)

target_link_libraries(CycleSlipDetection PRIVATE 
    Threads::Threads 
    absl::strings
    absl::base
    absl::flat_hash_map
    absl::statusor
    absl::status
    absl::flags_parse
    absl::flags
    absl::flags_usage
)

# 7. 编译警告配置 (区分 MSVC 和 GCC/Clang)
if (MSVC)
    target_compile_options(CycleSlipDetection PRIVATE /W4)
else()
    target_compile_options(CycleSlipDetection PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 8. (可选) 如果你以后使用 Eigen 等数学库，在此处添加
# find_package(Eigen3 REQUIRED)
# target_link_libraries(CycleSlipDetection PRIVATE Eigen3::Eigen)