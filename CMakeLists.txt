cmake_minimum_required(VERSION 3.5)
project(V8Performance)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-stdlib=libc++ ")
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)
file (GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)

function (find_include_dirs out)
    file (GLOB_RECURSE headers ${ARGN})
    foreach (path ${headers})
        get_filename_component (dir ${path} PATH)
        list (APPEND include_dirs ${dir})
    endforeach()
    list (REMOVE_DUPLICATES include_dirs)
    set (${out} ${include_dirs} PARENT_SCOPE)
endfunction()

find_include_dirs(includes third_party/v8/include/*.h)
include_directories(${includes})
file (GLOB_RECURSE v8 third_party/v8/mac/*.a)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/JSTest.js
        COMMAND ${CMAKE_COMMAND} -E copy ${V8Performance_SOURCE_DIR}/src/js/JSTest.js ${CMAKE_BINARY_DIR}/JSTest.js
        MAIN_DEPENDENCY ${V8Performance_SOURCE_DIR}/src/js/JSTest.js)
add_custom_target(JSTest ALL DEPENDS ${CMAKE_BINARY_DIR}/JSTest.js)

add_executable(Main ${SOURCE_FILES})

target_link_libraries(Main ${v8})