project(ChatOnlineServer)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
include_directories(/home/jiyuhang/git/muduo)
include_directories(/home/jiyuhang/git/rapidjson/include)
#include_directories(/root/program/muduo)
LINK_DIRECTORIES(/home/jiyuhang/git/build/release/lib)
#LINK_DIRECTORIES(/root/program/build/release/lib)

#add_library(libmuduo_net.a STATIC IMPORTED)
#add_library(libmuduo_base.a STATIC IMPORTED)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} muduo_net muduo_base pthread)

