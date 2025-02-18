
# ${CMAKE_CURRENT_LIST_DIR} is a variable in CMake that represents the absolute path to the directory where the CMake list file 
# (such as the CMakeLists.txt or.cmake file) is currently being processed 

message(STATUS  "in generator-cfg.cmake, CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")

app_include_generator(NAME msg_parser
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../code_gen/msg_parser"
    OUTPUT_DIRECTORY "msg_parser"
    COMMAND "python"
    SCRIPT_NAME "generator.py"
    ARGS "--build-output=${CMAKE_CURRENT_LIST_DIR}/../build/${BUILD_TYPE}"
         "--output-dir=${CMAKE_CURRENT_LIST_DIR}/../src/gen/msg_parser"
    COMMENT ""
)
