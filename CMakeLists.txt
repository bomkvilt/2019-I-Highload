cmake_minimum_required(VERSION 3.0)
#~~~~~~~~~~~~~~~~~~~~~~~| >> setup project |
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH}) 
INCLUDE("cmake/setup.cmake")
INCLUDE("cmake/units.cmake")
Init_solution("Highload")
SetupEvniroment()
#~~~~~~~~~~~~~~~~~~~~~~~| >> init project |
SUBDIRS(internal)
SUBDIRS(cmd     )
