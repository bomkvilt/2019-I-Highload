
# download data library
GN_Download_project(root "mmap" "./loader.cmake")

# date module
GB_Module( mmap 
    Public      "${root}/src/include"
    Mode        "headers"
    bFlat   on)
