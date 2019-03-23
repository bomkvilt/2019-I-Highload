cmake_minimum_required(VERSION 3.0)
#~~~~~~~~~~~~~~~~~~~~~~~| >>  |
include("cmake/setup.cmake")
include("cmake/gremlin/gremlin.cmake")

set(GN_bDebug off)
set(GN_bTests on)
set(GN_bUnity on)
SetupEvniroment()
GN_Solutoin("Highload")

#~~~~~~~~~~~~~~~~~~~~~~~| >>  |
GN_Subprojects(
    internal
    cmd)
#~~~~~~~~~~~~~~~~~~~~~~~| >>  |
GN_Configure()
