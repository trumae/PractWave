cmake_minimum_required (VERSION 2.8) 
project (PractWave3)

SET(VERSION_SERIES 0)
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 3)

#c++ flags
list( APPEND CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -g")

FIND_PACKAGE(Wt REQUIRED Wt HTTP)
FIND_PACKAGE(Boost)

add_subdirectory (utils) 
#add_subdirectory (widgets)
#add_subdirectory (apps) 
#add_subdirectory (logic)
#add_subdirectory (core)
#add_subdirectory (cabureAds)
