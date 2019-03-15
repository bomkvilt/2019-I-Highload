cmake_minimum_required(VERSION 3.0)
#~~~~~~~~~~~~~~~~~~~~~~~| >>  |
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH}) 
include("cmake/setup.cmake")
include("cmake_boggart/boggart.cmake")

set(UN_bDebug off)
set(UN_bTests on)
SetupEvniroment()
CreateSolution("Highload")
#~~~~~~~~~~~~~~~~~~~~~~~| >>  |
SUBDIRS(internal)
SUBDIRS(cmd     )
#~~~~~~~~~~~~~~~~~~~~~~~| >>  |
ProcessSolution()
