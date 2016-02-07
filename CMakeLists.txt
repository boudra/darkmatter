cmake_minimum_required(VERSION 2.6)
project(darkmatter)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/tests)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(VENDOR_DIR ${CMAKE_SOURCE_DIR}/vendor)
SET(ENABLE_TESTS 1 CACHE BOOL "Compile tests")

file(GLOB_RECURSE darkmatter_files "src/*.cpp")
file(GLOB_RECURSE test_files "src/*_test.cpp")

foreach(test ${test_files})
    list(REMOVE_ITEM darkmatter_files ${test})
endforeach(test)

include_directories(${darkmatter_SOURCE_DIR}/src ${VENDOR_DIR})

add_library(darkmatter SHARED ${darkmatter_files})

if(${ENABLE_TESTS})
    foreach(test ${test_files})
        get_filename_component(target ${test} NAME_WE)
        add_executable(${target} ${test})
        add_dependencies(${target} darkmatter)
        target_link_libraries(${target} darkmatter)
    endforeach(test)
endif()

find_package(OpenGL)
target_link_libraries(darkmatter ${OPENGL_LIBRARIES})

if(UNIX)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -O0 -Wall -Wno-long-long -g -pedantic -std=c++11 --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi")

    find_package(PkgConfig)

    pkg_check_modules(SDL2 sdl2)
    pkg_check_modules(IL IL)
    pkg_check_modules(ILU ILU)
    pkg_check_modules(FREETYPE freetype2)
    pkg_check_modules(GLEW glew)
    find_package(CPPFORMAT)

endif(UNIX)

if(WIN32)

    find_package(DevIL)
    find_package(SDL2)
    find_package(GLEW)
    find_package(FreeType)

endif(WIN32)


target_link_libraries(darkmatter ${GLEW_LIBRARIES})
include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(darkmatter ${SDL2_LIBRARIES})
target_link_libraries(darkmatter ${SDL2MAIN_LIBRARIES})

include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(darkmatter ${FREETYPE_LIBRARIES})
include_directories(${FREETYPE_INCLUDE_DIRS})

target_link_libraries(darkmatter ${ILU_LIBRARIES})
target_link_libraries(darkmatter ${IL_LIBRARIES})
include_directories(${IL_INCLUDE_DIRS})

target_link_libraries(darkmatter ${CPPFORMAT_LIBRARIES})
include_directories(${CPPFORMAT_INCLUDE_DIRS})

