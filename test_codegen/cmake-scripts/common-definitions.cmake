message(STATUS  "in common-definitions.cmake, CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")

# 定义版本号
set(APP_VERSION "0.1")

# Set compiler properties
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-O2 -g -std=c++17")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MOD_TYPE_GEN  "GEN")
set(GEN_BASE_PATH   "${CMAKE_CURRENT_LIST_DIR}/../src/gen")


add_definitions(-DAPP_VERSION=\"${APP_VERSION}\")