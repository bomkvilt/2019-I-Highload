cmake_minimum_required(VERSION 3.12)

project(date-loader NONE)

# date library
include(ExternalProject)
ExternalProject_Add( date
    GIT_REPOSITORY    https://github.com/HowardHinnant/date.git
    GIT_TAG           master
    SOURCE_DIR        "${root}/src"
    BINARY_DIR        "${root}/build"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
    TEST_COMMAND      ""
    )

