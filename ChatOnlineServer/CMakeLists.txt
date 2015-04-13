project(ChatOnlineServer)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
include_directories(/home/jiyuhang/program/muduo)
include_directories(/root/program/muduo)
LINK_DIRECTORIES(/home/jiyuhang/program/build/release/lib)
LINK_DIRECTORIES(/root/program/build/release/lib)

#add_library(libmuduo_net.a STATIC IMPORTED)
#add_library(libmuduo_base.a STATIC IMPORTED)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} muduo_net muduo_base pthread)

