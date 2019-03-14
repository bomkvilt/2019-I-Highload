#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Units' settings
# common settings
set(UN_bDebug 	        off		        CACHE BOOL "Print unit debug information")
set(UN_bTests 	        on		        CACHE BOOL "Enable tests")
set(UN_cpp_version 		17		        CACHE STRING "C++ standart")
# directory settings
set(UN_dir_private	    "Private"       CACHE STRING "Unit private directory")
set(UN_dir_public	    "Public"        CACHE STRING "Unit public directory")
set(UN_dir_data		    "Data"	        CACHE STRING "Unit data directory")
set(UN_dir_test		    "Test"	        CACHE STRING "Unit test directory")
# test settings
set(UN_tests_filter	    "_tests_"       CACHE STRING "Filter name for all test projects")
set(UN_tests_target     "tests_exe"     CACHE STRING "test target")
set(UN_tests_test       "tests_test"    CACHE STRING "test name")

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Initialisatoin

## create a solution and perform it's setting up
#
macro(CreateSolution _name)
    # create a solution project
    project(${_name} NONE)
    set(CMAKE_CXX_STANDARD ${UN_cpp_version})
    
    # setup envitroment
    set_property(GLOBAL PROPERTY UN_test_projects "")
    set(UN_dir_solution ${PROJECT_SOURCE_DIR})
    UN_setup_policies()

    # test enviroment
    set(UN_test_includes "" CACHE STRING "")
    if (UN_bTests)
        UN_setup_test_enviroment()
        endif()
    endmacro()

## perform a final solution processing
#
macro(ProcessSolution)
    if (UN_bTests)
        UN_setup_tests()
        endif()
    endmacro()

## add solution subrirectory
#
macro(AddSubdirectory _path)
    SUBDIRS(${_path})
    endmacro()

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Units

## creates a unit with the folowing params:
#   \ Name                      - unit name
#   \ Modules           = {}    - list of depending modules             | inherits
#   \ Private_includes  = {}    - list of private external include dirs | 
#   \ Public_includes   = {}    - list of public  external include dirs | inherits
#   \ Libraries         = {}    - list of depending external libs       | inherits
#   \ Definitions       = {}    - list of preprocessor defenitions      | inherits
#   \ bFlat             = off   - [on|off] whether the unit uses separated public/private/test directories
#   \ Mode              = lib   - [...] type of unit will be built
#       \ lib       - create a static library
#       \ app       - create an executable
#       \ headers   - create a target with no binary output
function(CreateUnit)
    # parse arguments
    UN_parse_arguments(${ARGN})
    UN_check_arguments()
    # create cmake project
    UN_create_project()
    # setup directories
    UN_setup_directories()
    if (UN_bDebug)
        message("")
        message("|${Name}::${Mode}::${dir_root}|")
        endif()
    # setup cmake project and find all required values
    UN_setup_modules(modules "${Modules}" ${Name})
    UN_setup_libs(libs "${Libraries}" "${modules}" ${Name})
    UN_setup_source(files_private ${dir_private} ${dir_root})
    UN_setup_source(files_public  ${dir_public}  ${dir_root})
    UN_setup_source(files_test    ${dir_test}    ${dir_root})
    UN_setup_includes(
        Includes    ${dir_private} ${dir_public} ${dir_test} ${Private_includes} ${Public_includes} ${UN_test_includes}
        Modules     ${modules})
        
    # create a target
    if (${Mode} STREQUAL "lib")
        add_library(${Name} 
            ${files_public} 
            ${files_private}
            ${files_test})
        set(bLink on)
        endif()
    if (${Mode} STREQUAL "app")
        add_executable(${Name} 
            ${files_public} 
            ${files_private}
            ${files_test})
        target_link_libraries(${Name} ${libs})
        set_target_properties(${Name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${UN_dir_solution})
        set(bLink off)
        endif()
    if (${Mode} STREQUAL "headers")
        add_custom_target(${Name} 
            SOURCES ${files_public} 
            SOURCES ${files_private}
            SOURCES ${files_test})
        set(bLink off)
        endif()
    # setup target properties
    UN_setup_definitions(definitions ${Name} ${dir_root} ${dir_data} "${Definitions}" "${modules}")
    UN_setup_filter(${Name} ${dir_root})
    UN_define_unit(${Name} 
        "${dir_public}"
        "${Public_includes}"
        "${libs}"
        "${modules}"
        "${definitions}"
        "${bLink}")
    endfunction()

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Unit utils
#~~~~~~~~~~~~~| defines

set(UN_module_suffix "_MOD")
set(UN_public_suffix "_PUB")
set(UN_libraty_suffix "_LIB")
set(UN_linkage_suffix "_LNK")
set(UN_definition_suffix "_DEF")

#~~~~~~~~~~~~~| macro

macro(UN_parse_arguments)
    set(_one_value_args     bFlat Mode Name)
    set(_multy_value_args   Modules Private_includes Public_includes Libraries Definitions)
    cmake_parse_arguments(
        ""
        ""
        "${_one_value_args}"
        "${_multy_value_args}"
        ${ARGN})
    foreach(variable ${_options} ${_one_value_args} ${_multy_value_args})
        set(${variable} "${_${variable}}")
        endforeach()
    endmacro()

macro(UN_check_arguments)
    if (NOT Name)
        message(FATAL_ERROR "Unit name must be set")
        endif()
    if (NOT bFlat)
        set(bFlat off)
        endif()
    if (NOT Mode)
        set(Mode "lib")
        endif()
    endmacro()

macro(UN_create_project)
    project(${Name})
    set(CMAKE_CXX_STANDARD ${UN_cpp_version})
    endmacro()

macro(UN_setup_directories)
    set(dir_root ${PROJECT_SOURCE_DIR})
    if (bFlat)
        set(dir_private ".")
        set(dir_public  ".")
        set(dir_data    ".")
        set(dir_test    ".")
    else()
        set(dir_private ${UN_dir_private})
        set(dir_public  ${UN_dir_public})
        set(dir_data    ${UN_dir_data})
        set(dir_test    ${UN_dir_test})
        endif()
    endmacro()

#~~~~~~~~~~~~~| functions

function(UN_setup_modules _all_modules _modules _unit_name)
    # find all depending modules to a scope list
    set(_tmp_list ${_modules})
    foreach(_module ${_modules})
        get_property(_submodules GLOBAL PROPERTY ${_module}${UN_module_suffix})
        list(APPEND _tmp_list ${_submodules})
        endforeach()
    list(REMOVE_DUPLICATES _tmp_list)
    # print the found modules
    if (UN_bDebug)
		message("--   modules: \t ${_tmp_list}")
        endif()
    # move the modules to a parent scope variable
    UN_return(${_all_modules} "${_tmp_list}")
    endfunction()

function(UN_setup_libs _all_libs _libs _modules _unit_name)
    # find all depending libs to a scope list
    set(_tmp_list ${_libs} "")
    foreach(_module ${_modules})
        get_property(_sublibs GLOBAL PROPERTY ${_module}${UN_libraty_suffix})
        get_property(_bLink   GLOBAL PROPERTY ${_module}${UN_linkage_suffix})
        list(APPEND _tmp_list ${_sublibs} ${_module})
        if (NOT _bLink)
            list(APPEND _rm_list ${_module})
            endif()
        endforeach()
    list(REMOVE_DUPLICATES _tmp_list)
    list(REMOVE_ITEM _tmp_list ${_rm_list} "")
    # print the found libs
    if (UN_bDebug)
        message("-- libraries: \t ${_tmp_list}")
        endif()
    # move the libs to a parent scope variable
    UN_return(${_all_libs} "${_tmp_list}")
    endfunction()

function(UN_setup_source _sources _directory _root)
    # find files and add them to the source group
    file(GLOB_RECURSE _files "${_directory}/*")
    source_group(TREE ${_root} FILES ${_files})
    # move the files to a parent scope variable
    UN_return(${_sources} "${_files}")
    endfunction()

function(UN_setup_includes)
    # parse arguments
    set(_prefix ""      )
    set(_options        )
    set(_one_value_args )
    set(_multy_value_args   Includes Modules)
    cmake_parse_arguments(
        "${_prefix}"
        "${_options}"
        "${_one_value_args}"
        "${_multy_value_args}"
        ${ARGN})
    # find include directories and include them
    set(_includes ${_Includes})
    foreach(_module ${_Modules})
        get_property(_public GLOBAL PROPERTY ${_module}${UN_public_suffix})
        list(APPEND _includes ${_public})
        endforeach()
    include_directories(${_includes})
    endfunction()

function(UN_setup_filter _unit_name _unit_root)
    file(RELATIVE_PATH _category "${UN_dir_solution}" "${_unit_root}/..")
    set_target_properties(${_unit_name} PROPERTIES FOLDER "${_category}")
    endfunction()

function(UN_define_unit _unit_name _dir_public _Public_includes _libs _modules _definitions _bLink)
    # absolute paths
    get_filename_component(_abs_public "${_dir_public}" ABSOLUTE)
    get_filename_component(_abs_includes "${_Public_includes}" ABSOLUTE)
    set(_publics ${_abs_public} ${_abs_includes})
    # set global properties
    set_property(GLOBAL PROPERTY ${_unit_name}${UN_module_suffix}     ${_modules}       )
	set_property(GLOBAL PROPERTY ${_unit_name}${UN_public_suffix}     ${_publics}       )
	set_property(GLOBAL PROPERTY ${_unit_name}${UN_libraty_suffix}    ${_libs}          )
    set_property(GLOBAL PROPERTY ${_unit_name}${UN_linkage_suffix}    ${_bLink}         )
    set_property(GLOBAL PROPERTY ${_unit_name}${UN_definition_suffix} ${_definitions}   )
    # register to tests
    if (_bLink)
        get_property(_modules GLOBAL PROPERTY UN_test_projects)
        list(APPEND _modules ${_unit_name})
        set_property(GLOBAL PROPERTY UN_test_projects ${_modules})
        endif()
    endfunction()

function(UN_setup_definitions _all_defines _unit_name _unit_root _dir_data _definitions _modules)
    # get inherited defines
    foreach(_module ${_modules})
        get_property(_subdefs GLOBAL PROPERTY ${_module}${UN_definition_suffix})
        list(APPEND _definitions ${_subdefs} "")
        endforeach()
    list(REMOVE_DUPLICATES _definitions)
    list(REMOVE_ITEM _definitions "")
    UN_return(${_all_defines} "${_definitions}")
    
    # data directory
    file(RELATIVE_PATH _category "${UN_dir_solution}" "${_unit_root}/..")
    list(APPEND _definitions "-DDATA_DIR=${_category}/${_dir_data}")
    
    # print debug
    if (UN_bDebug)
        message("--   defines: \t ${_definitions}")
        endif()
    
        # apply defenitions
    foreach(_define ${_definitions})
        add_definitions(${_define})
        endforeach()
    endfunction()

macro(UN_return _name _value)
    set(${_name} "${_value}" PARENT_SCOPE)
    endmacro()

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Solution utils

macro(UN_setup_policies)
	cmake_policy(SET CMP0074 NEW)
	cmake_policy(SET CMP0079 NEW)
    endmacro()

function(UN_setup_test_enviroment)
	# download gtest to a build directory
	UN_download_gtest()
	# setup gtest enviroment
	include(GoogleTest)
	enable_testing()
	set(gtest_force_shared_crt off CACHE BOOL "" FORCE)
	add_subdirectory(
		"${CMAKE_CURRENT_BINARY_DIR}/googletest-src"
		"${CMAKE_CURRENT_BINARY_DIR}/googletest-build"
        EXCLUDE_FROM_ALL)
    set(UN_test_includes "${gtest_SOURCE_DIR}/include" CACHE STRING "" FORCE)
    endfunction()

function(UN_setup_tests)
    # copy a testing app to build directory and configure it
    AddSubdirectory("cmake/units")
    endfunction()
    
function(UN_download_gtest)
	if (NOT Units_test_downloaded)
		# download and unpack googletest at configure time
        configure_file("cmake/units/test.cmake" "googletest-download/CMakeLists.txt")
        execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        	RESULT_VARIABLE result
        	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/googletest-download")
        if(result)
        	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
        	endif()
        execute_process(COMMAND ${CMAKE_COMMAND} --build .
        	RESULT_VARIABLE result
        	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/googletest-download")
        if(result) 
        	message(FATAL_ERROR "Build step for googletest failed: ${result}")
        	endif()
        # prevent next updates
		set(Units_test_downloaded on CACHE BOOL "" FORCE)
    endif()
    endfunction()
