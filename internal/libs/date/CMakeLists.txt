
# download data library
GN_Download_project(root "date" "./loader.cmake")

# date module
GB_Module( date 
    Public      "${root}/src/include"
    Mode        "headers"
    Definitions "-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
    bFlat   on)
