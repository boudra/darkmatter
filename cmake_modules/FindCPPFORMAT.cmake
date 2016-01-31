FIND_PATH(CPPFORMAT_INCLUDE_DIRS cppformat/format.h
  PATHS ${CMAKE_SOURCE_DIR}/thirdparty
)

# MESSAGE("CPPFORMAT_INCLUDE_DIRS is ${CPPFORMAT_INCLUDE_DIRS}")

FIND_LIBRARY(CPPFORMAT_LIBRARIES
  NAMES format libcppformat.a
  PATH_SUFFIXES lib
  PATHS
  ${CMAKE_SOURCE_DIR}/thirdparty/*
)

# MESSAGE("CPPFORMAT_LIBRARIES is ${CPPFORMAT_LIBRARIES}")
