#INCLUDE(FindPackageHandleStandardArgs)
#
#MESSAGE("${CMAKE_SOURCE_DIR}/thirdparty/DevIL-1.7.8/include")
#
#FIND_PATH(IL_INCLUDE_DIRS IL/il.h 
#  PATH_SUFFIXES include
#  PATHS
#  ${CMAKE_SOURCE_DIR}/thirdparty/*
#)
#
#MESSAGE("IL_INCLUDE_DIRS is ${IL_INCLUDE_DIRS}")
#
#FIND_LIBRARY(IL_LIBRARIES
#  NAMES IL DEVIL
#  PATH_SUFFIXES lib64 lib lib32
#  PATHS
#  ${CMAKE_SOURCE_DIR}/thirdparty/*
#)
#
#MESSAGE("IL_LIBRARIES is ${IL_LIBRARIES}")
#
#FIND_LIBRARY(ILU_LIBRARIES
#  NAMES ILU
#  PATH_SUFFIXES lib64 lib lib32
#  PATHS
#  ${CMAKE_SOURCE_DIR}/thirdparty/*
#)
#
#MESSAGE("ILU_LIBRARIES is ${ILU_LIBRARIES}")
#
#FIND_PACKAGE_HANDLE_STANDARD_ARGS(IL DEFAULT_MSG 
#                                  IL_LIBRARIES ILU_LIBRARIES
#                                  IL_INCLUDE_DIRS)
