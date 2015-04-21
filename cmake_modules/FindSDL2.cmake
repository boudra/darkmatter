INCLUDE(FindPackageHandleStandardArgs)

FIND_PATH(SDL2_INCLUDE_DIRS SDL.h 
  PATH_SUFFIXES include
  PATHS
  ${CMAKE_SOURCE_DIR}/thirdparty/*
)

#MESSAGE("SDL2_INCLUDE_DIRS is ${SDL2_INCLUDE_DIRS}")

FIND_LIBRARY(SDL2_LIBRARIES
  NAMES SDL2
  PATH_SUFFIXES lib64 lib lib32
  PATHS
  ${CMAKE_SOURCE_DIR}/thirdparty/*
)

#MESSAGE("SDL2_LIBRARIES is ${SDL2_TEMP_1}")

FIND_LIBRARY(SDL2MAIN_LIBRARIES
  NAMES SDL2main
  PATH_SUFFIXES lib64 lib lib32
  PATHS
  ${CMAKE_SOURCE_DIR}/thirdparty/*
)

#MESSAGE("SDL2_LIBRARIES is ${SDL2_TEMP_2}")

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2 DEFAULT_MSG 
                                  SDL2_LIBRARIES
								  SDL2MAIN_LIBRARIES
                                  SDL2_INCLUDE_DIRS)
