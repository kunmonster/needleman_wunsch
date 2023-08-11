cmake_minimum_required(VERSION 3.0.0)
project(needleman-wunsch VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()



# 单个目录单个源文件
# aux_source_directory(<Dir> <variable>) 当有多个源文件时,此命令会查找指定目录的源文件，将结果存入指定变量名
# aux_source_directory(. DIR_SRCS)

# 添加子目录
add_subdirectory(src)

# 头文件目录，一般在顶层目录添加
include_directories(include)


# 指定生成目标
add_executable(needleman-wunsch main.cpp)

# 添加链接库
target_link_libraries(needleman-wunsch Align)

