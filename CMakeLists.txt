# Build script for Simulty
# ==============================================================================

# Require at least CMake version 2.6
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF(WIN32)
  SET(CMAKE_CXX_FLAGS "-D__GTHREAD_HIDE_WIN32API -g -Wall")
  SET(CMAKE_CXX_FLAGS_DEBUG "-D__GTHREAD_HIDE_WIN32API -g -Wall")  
ELSE(WIN32)
  SET(CMAKE_CXX_FLAGS "-g -Wall -pendantic")
  SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -pedantic")
ENDIF(WIN32)

SET(CMAKE_COLOR_MAKEFILE ON) 
SET(CMAKE_VERBOSE_MAKEFILE OFF)

PROJECT(Simulty)

SUBDIRS(src)


