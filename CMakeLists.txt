
cmake_minimum_required(VERSION 3.10)
project(demoapp)
message(STATUS "in myapp show PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

#重新定义当前目标的源文件的__FILE__宏
function(redefine_file_macro targetname)
    #获取当前目标的所有源文件
    get_target_property(source_files "${targetname}" SOURCES)
    #遍历源文件
    foreach(sourcefile ${source_files})
        #获取当前源文件的编译参数
        message(STATUS "file:${sourcefile}")
        get_property(defs SOURCE "${sourcefile}"
            PROPERTY COMPILE_DEFINITIONS)
        #获取当前文件的绝对路径
        get_filename_component(filepath "${sourcefile}" ABSOLUTE)
        #将绝对路径中的项目路径替换成空,得到源文件相对于项目路径的相对路径
        string(REPLACE ${PROJECT_SOURCE_DIR}/ "${PROJECT_NAME}/" relpath ${filepath})
        #将我们要加的编译参数(__FILE__定义)添加到原来的编译参数里面
        list(APPEND defs "__FILE__=\"${relpath}\"")
        #重新设置源文件的编译参数
        set_property(
            SOURCE "${sourcefile}"
            PROPERTY COMPILE_DEFINITIONS ${defs}
            )

    endforeach()
endfunction()

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-O2 -g -std=c++17")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "in myapp show CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "in myapp show CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

# 添加 tinylog 和 tinynet 子目录
add_subdirectory(${CMAKE_SOURCE_DIR}/../tinylog ${CMAKE_CURRENT_BINARY_DIR}/tinylog)
add_subdirectory(${CMAKE_SOURCE_DIR}/../tinynet ${CMAKE_CURRENT_BINARY_DIR}/tinynet)

file(GLOB ECHO_SERVER_SOURCES "src/echo_server.cpp")
file(GLOB ECHO_CLIENT_SOURCES "src/echo_client.cpp")
file(GLOB HTTP_SERVER_TEST "src/http_server_test.cpp")
file(GLOB WEBSOCKET_SERVER_TEST "src/websocket_server_test.cpp")
file(GLOB WEBSOCKET_CLIENT_TEST "src/websocket_client_test.cpp")
file(GLOB CAN_WAKE_UP_TEST "src/can_wakeup.cpp")

# 创建可执行文件
add_executable(echo_server ${ECHO_SERVER_SOURCES})
redefine_file_macro(echo_server)
add_executable(echo_client ${ECHO_CLIENT_SOURCES})
redefine_file_macro(echo_client)
add_executable(http_server ${HTTP_SERVER_TEST})
redefine_file_macro(http_server)
add_executable(websocket_server ${WEBSOCKET_SERVER_TEST})
redefine_file_macro(websocket_server)
add_executable(websocket_client ${WEBSOCKET_CLIENT_TEST})
redefine_file_macro(websocket_client)
add_executable(can_wakeup ${CAN_WAKE_UP_TEST})
redefine_file_macro(can_wakeup)

# 链接库
target_link_libraries(echo_server loglib netlib)
target_link_libraries(echo_client loglib netlib)
target_link_libraries(http_server loglib netlib)
target_link_libraries(websocket_server loglib netlib)
target_link_libraries(websocket_client loglib netlib)
target_link_libraries(can_wakeup loglib netlib)

# 设置可执行文件输出目录
set_target_properties(echo_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(echo_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(http_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(websocket_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(websocket_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(can_wakeup PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
