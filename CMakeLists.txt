cmake_minimum_required (VERSION 2.6) 
project (PractWave2)

#inclui suporte a c++11
list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g")

add_subdirectory (utils) 
add_subdirectory (widgets)
add_subdirectory (apps) 
add_subdirectory (logic)
add_subdirectory (core)
add_subdirectory (cabureAds)