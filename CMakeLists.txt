
cmake_minimum_required(VERSION 3.10)
project(MyProject)
message(STATUS "in myapp show PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-O2 -g -std=c++11")

message(STATUS "in myapp show CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "in myapp show CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

# 添加 tinylog 和 tinynet 子目录
add_subdirectory(${CMAKE_SOURCE_DIR}/../tinylog ${CMAKE_CURRENT_BINARY_DIR}/tinylog)
add_subdirectory(${CMAKE_SOURCE_DIR}/../tinynet ${CMAKE_CURRENT_BINARY_DIR}/tinynet)

file(GLOB ECHO_SERVER_SOURCES "src/echo_server.cpp")
file(GLOB ECHO_CLIENT_SOURCES "src/echo_client.cpp")

# 创建可执行文件
add_executable(echo_server ${ECHO_SERVER_SOURCES})
add_executable(echo_client ${ECHO_CLIENT_SOURCES})

# 链接库
target_link_libraries(echo_server loglib netlib)
target_link_libraries(echo_client loglib netlib)

# 设置可执行文件输出目录
set_target_properties(echo_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(echo_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
