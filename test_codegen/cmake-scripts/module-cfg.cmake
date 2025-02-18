message(STATUS  "in module-cfg.cmake, CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")

# gen modules
# PATH relative to:     ${CMAKE_CURRENT_LIST_DIR}/../src/gen
app_include_module(NAME "sysm_msg_parser" TYPE ${MOD_TYPE_GEN} PATH "sysm_msg_parser")
