cmake_minimum_required(VERSION 3.5)
project(V8Performance)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-stdlib=libc++ ")
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)
file (GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)

if (CMAKE_BUILD_TYPE STREQUAL Release)
    set(BUILD_TYPE darwin-x64-release)
else ()
    set(BUILD_TYPE darwin-x64-debug)
endif ()

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
include_directories(${includes} src)
file (GLOB_RECURSE v8_lib third_party/v8/${BUILD_TYPE}/*.a)

set(RESOURCE_FILES)

function(copy_file from to)
    set(temp ${RESOURCE_FILES})
    list(APPEND temp ${to})
    set(RESOURCE_FILES ${temp} PARENT_SCOPE)
    add_custom_command(OUTPUT ${to}
            COMMAND ${CMAKE_COMMAND} -E copy ${from} ${to}
            MAIN_DEPENDENCY ${from})
endfunction()

copy_file(${CMAKE_SOURCE_DIR}/src/js/JSTest.js ${CMAKE_BINARY_DIR}/JSTest.js)
#copy natives_blob.bin and snapshot_blob.bin which v8 requires to the root of binary dir.
copy_file(${CMAKE_SOURCE_DIR}/third_party/v8/${BUILD_TYPE}/natives_blob.bin ${CMAKE_BINARY_DIR}/natives_blob.bin)
copy_file(${CMAKE_SOURCE_DIR}/third_party/v8/${BUILD_TYPE}/snapshot_blob.bin ${CMAKE_BINARY_DIR}/snapshot_blob.bin)

add_custom_target(CopyResource ALL DEPENDS ${RESOURCE_FILES})

add_executable(Main ${SOURCE_FILES})

if (MSVC)
    set(cc_flags "/w /GR-")
else()
    set(cc_flags "-w -fno-rtti -fno-exceptions")
endif()
set_target_properties(Main PROPERTIES
        COMPILE_FLAGS ${cc_flags}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN true)

target_link_libraries(Main ${v8_lib})