cmake_minimum_required(VERSION 3.13)

project(sysmonitor_client LANGUAGES CXX)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER "ccache")
endif()

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} "")

# CMAKE_BUILD_TYPE is set to None and not used. 
# Build type is managed by BUILD_TYPE instead.
set(CMAKE_BUILD_TYPE None)


# BUILD_TYPE: DEBUG or RELEASE
# use BUILD_TYPE_DEBUG/BUILD_TYPE_RELEASE in source files
target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_TYPE_${BUILD_TYPE})

message(STATUS  "in main CMakeLists.txt, PROJECT_ROOT_DIR: ${PROJECT_ROOT_DIR}")

include(${CMAKE_CURRENT_LIST_DIR}/cmake-scripts/common-definitions.cmake)
target_compile_definitions(${PROJECT_NAME} PUBLIC APP_VERSION=\"${APP_VERSION}\")

# Include NVOS specific helper functions
include(${CMAKE_CURRENT_LIST_DIR}/cmake-scripts/helper-func.cmake)


target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)
file(GLOB MAIN_SRC_FILES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
target_sources(${PROJECT_NAME} PUBLIC ${MAIN_SRC_FILES})


# add_custom_target(exect_before_compile COMMENT "")
# add_dependencies(${PROJECT_NAME} exect_before_compile)
# add_custom_command(TARGET ${PROJECT_NAME}
#             POST_BUILD
#             COMMENT "")


include(${CMAKE_CURRENT_LIST_DIR}/cmake-scripts/generator-cfg.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake-scripts/module-cfg.cmake)



# 添加 tinylog 和 tinynet 子目录
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../tinylog ${CMAKE_CURRENT_BINARY_DIR}/tinylog)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../tinynet ${CMAKE_CURRENT_BINARY_DIR}/tinynet)
target_link_libraries(${PROJECT_NAME} loglib netlib)
