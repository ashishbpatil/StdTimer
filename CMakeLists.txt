cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(StdTimer)
set(CMAKE_BUILD_TYPE Release)
 
# Bring the headers
include_directories(include)
 
# Setup project source files
SET (PROJECT_SOURCES
     StdTimer.cpp
     StdTimer.h)
     
 
#Generate the shared library from the sources
add_library(StdTimer SHARED ${PROJECT_SOURCES})
 
# Set the location for library installation -- i.e., /usr/lib in this case
# Use "sudo make install" to apply
install(TARGETS StdTimer DESTINATION /usr/lib)
