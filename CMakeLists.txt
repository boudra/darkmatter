cmake_minimum_required(VERSION 2.6)
project(game)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB game_files "src/*.cpp" "src/*/*.cpp" "src/*.hpp" "src/*/*.hpp")

macro(GroupSources curdir)

   file(GLOB children RELATIVE ${game_SOURCE_DIR}/${curdir} ${game_SOURCE_DIR}/${curdir}/*)

   foreach(child ${children})
   
          if(IS_DIRECTORY ${game_SOURCE_DIR}/${curdir}/${child})
                  GroupSources(${curdir}/${child})
          else()
				string(REPLACE "/" "\\" groupname ${curdir})
				#string(REPLACE "src" "Source" groupname ${groupname})
				source_group(${groupname} FILES ${game_SOURCE_DIR}/${curdir}/${child})
			
          endif()

   endforeach()

endmacro()

GroupSources(src)

include_directories(${game_SOURCE_DIR}/src)

add_executable(game ${game_files})

find_package(OpenGL)
target_link_libraries(game ${OPENGL_LIBRARIES})

if(UNIX)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -O0 -Wall -Wno-long-long -g -pedantic -std=c++11")
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


target_link_libraries(game ${GLEW_LIBRARIES})
include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(game ${SDL2_LIBRARIES})
target_link_libraries(game ${SDL2MAIN_LIBRARIES})

include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(game ${FREETYPE_LIBRARIES})
include_directories(${FREETYPE_INCLUDE_DIRS})

target_link_libraries(game ${ILU_LIBRARIES})
target_link_libraries(game ${IL_LIBRARIES})
include_directories(${IL_INCLUDE_DIRS})

target_link_libraries(game ${CPPFORMAT_LIBRARIES})
include_directories(${CPPFORMAT_INCLUDE_DIRS})

