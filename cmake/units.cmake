# Cmake code style:
# private macro/function -  <function_name>
# public  macro/function -  <Function_name>
# local   variable		 - _<variable_name>
# private variable		 -  <variable_name>
# public  variable		 -  <Variable_name>
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(Units_bDebug 	off			CACHE BOOL "Print unit debug information")
set(Units_bTests 	on			CACHE BOOL "Enable tests")
set(Units_cpp 		17			CACHE STRING "C++ standart")
set(Units_private	"Private"	CACHE STRING "Unit private directory")
set(Units_public	"Public" 	CACHE STRING "Unit public directory")
set(Units_data		"Data"		CACHE STRING "Unit data directory")
set(Units_test		"Test"		CACHE STRING "Unit test directory")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# define solution
#
macro(Init_solution _project_name) # -> ENGINE_ROOT
	project(${_project_name} NONE)
	set(CMAKE_CXX_STANDARD ${Units_cpp})
	set(ENGINE_ROOT ${PROJECT_SOURCE_DIR})

	setup_test_enviroment()
endmacro()

# define project
#
macro(Add_project _project_path)
	SUBDIRS(${_project_path})
endmacro()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# define new unit (module / application)
#
macro(Unit _name)
	#-------------------------------------------------# interface variables
	set(Modules)			# required modules
	set(Includes_private)	# include files from 3rdParty
	set(Includes)			# include files from 3rdParty
	set(Libs)				# .lib 	  files from 3tdParty
	set(bFlat off)			# disable public | private | data subdirs
	#-------------------------------------------------# internal variables
	set(unit_root)			# unit root directory
	set(public_dir)			# found includes for the unit
	set(private_dir)		# found sources for the unit
	set(data_dir)         	# folder with module resources
	set(test_dir)			# folder with test files
	set(modules)			# all project modules = Modules + Modules' modules + ...
	set(libs)				# unit's libs
	set(test_files)			# unit's test files
	set(public_files)		# unit's public files
	set(private_files)		# unit's private files
	set(unit_name ${_name})	# unit name
	#-------------------------------------------------# unit defenition
	project(${unit_name})	# create a cmake project
	set(unit_root ${PROJECT_SOURCE_DIR})
	set(CMAKE_CXX_STANDARD ${Units_cpp})
endmacro()

# use the unit as an module
#
macro(Module)
	setup_unit()
	add_library(${unit_name} 
		${public_files} 
		${private_files}
		${test_files}
	)
	setup_unit_end(on)
	setup_test()
endmacro()

# use the unit as an application
#
macro(Application)
	setup_unit()
	add_executable(${unit_name} 
		${public_files} 
		${private_files}
		${test_files}
	)
	target_link_libraries(${unit_name} ${libs})
	setup_properties()
	setup_unit_end(off)
endmacro()

# 
#
macro(HeaderLibrary)
	setup_unit()
	add_custom_target(${unit_name} 
		SOURCES ${public_files} 
		SOURCES ${private_files}
		SOURCES ${test_files}
	)
	setup_definitions()
	setup_unit_end(off)
endmacro()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

macro(setup_unit)
	setup_modules()
	setup_source()
	setup_includes()
	setup_definitions()
	setup_libs()
endmacro()

macro(setup_unit_end _bLink)
	setup_project_filter()
	defineUnit(${_bLink})
endmacro()

macro(setup_source)
	# code directories
	if (bFlat)
		set(private_dir "")
		set(public_dir	"")
		set(data_dir	"")
		set(test_dir	"")
	else()
		set(private_dir "${Units_private}/")
		set(public_dir  "${Units_public}/")
		set(data_dir    "${Units_data}/")
		set(test_dir	"${Units_test}/")
	endif()
	# find unit files
	file(GLOB_RECURSE private_files "${private_dir}*")
	file(GLOB_RECURSE public_files	"${public_dir}*")
	file(GLOB_RECURSE test_files	"${test_dir}*")
	# add the files into a unit's code tree
	source_group(TREE ${unit_root} FILES ${private_files})
	source_group(TREE ${unit_root} FILES ${public_files})
	source_group(TREE ${unit_root} FILES ${test_files})
endmacro()

macro(setup_modules)
	# get the unit's units
	set(modules "" ${Modules})
	# get the unit's units' units 
	foreach(_module ${Modules})
		get_property(_modules GLOBAL PROPERTY ${_module}_MOD)
		list(APPEND modules ${_modules})
	endforeach()
	# remove duplicates
	list(REMOVE_DUPLICATES modules)
	# print the units
	if (Units_bDebug)
		message("---|${unit_name} \t|${modules}")
	endif()
endmacro()

macro(setup_includes)
	# get unit's includes
	set(_includes ${public_dir} ${test_dir} ${private_dir} ${Includes_private} ${Includes} ${Units_test_inc})
	# get unit's units' includes
	foreach(_module ${modules})
		get_property(_unit_dir GLOBAL PROPERTY ${_module}_DIR)
		list(APPEND _includes ${_unit_dir}) 
	endforeach()
	# add the includes into the unit
	include_directories(${_includes})
endmacro()

macro(setup_libs)
	# get unit's libs
	set(libs "" ${Libs})
	# get unit's units' libs
	foreach(_module ${modules})
		get_property(_unit_lib GLOBAL PROPERTY ${_module}_LIB)
		get_property(_unit_lnk GLOBAL PROPERTY ${_module}_LNK)
		list(APPEND libs ${_unit_lib})
		if (NOT ${_unit_lnk})
			list(APPEND rems ${_module})
		endif()
	endforeach()
	# remove none-linking modules
	list(REMOVE_ITEM libs "" ${rems})
	# print the libs
	if (Units_bDebug)
		message("---{${unit_name} \t}${libs}")
	endif()
endmacro()

macro(setup_project_filter)
	set(_path "${unit_root}/..")
	file(RELATIVE_PATH _category "${ENGINE_ROOT}" "${_path}")
	set_target_properties(${unit_name} PROPERTIES FOLDER "${_category}")
endmacro()

macro(setup_properties)
	set_target_properties(${unit_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${ENGINE_ROOT})
endmacro()

macro(defineUnit _bLink)
	# absolute include paths
	get_filename_component(_public_dir "${public_dir}" ABSOLUTE)
	get_filename_component(_pubIncs    "${Includes}"   ABSOLUTE)
	# combine export states
	set(_public_dirs ${_public_dir} ${_pubIncs} )
	set(_public_libs ${unit_name}    ${Libs}     )
	# set global properties
	set_property(GLOBAL PROPERTY ${unit_name}_DIR ${_public_dirs})
	set_property(GLOBAL PROPERTY ${unit_name}_LIB ${_public_libs})
	set_property(GLOBAL PROPERTY ${unit_name}_MOD ${modules})
	set_property(GLOBAL PROPERTY ${unit_name}_LNK ${_bLink})
endmacro()

macro(setup_definitions)
	file(RELATIVE_PATH _category "${ENGINE_ROOT}" "${unit_root}")
	add_compile_definitions(DATA_DIR="${_category}/${data_dir}")
endmacro()

macro(setup_test)
	set(test_name "${unit_name}_exe")
	add_executable(${test_name}
		${public_files} 
		${private_files}
		${test_files}
	)
	gtest_discover_tests(${test_name})
	add_test(NAME ${test_name}_unit COMMAND ${test_name})
	target_link_libraries(${test_name} ${libs} ${Units_test_lib})
	set_target_properties(${test_name} PROPERTIES FOLDER "__tests__")
endmacro()

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

macro(setup_test_enviroment)
	# download gtest repo
	if (NOT Units_test_downloaded)
		# download and unpack googletest at configure time
		configure_file("cmake/test.cmake" "googletest-download/CMakeLists.txt")
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
	# 
	include(GoogleTest)
	# add a testing target
	enable_testing()
	# prevent overriding the parent project's compiler/linker
	set(gtest_force_shared_crt off CACHE BOOL "" FORCE)
	# add googletest directly to our build
	add_subdirectory(
		"${CMAKE_CURRENT_BINARY_DIR}/googletest-src"
		"${CMAKE_CURRENT_BINARY_DIR}/googletest-build"
		EXCLUDE_FROM_ALL)
	# define test framework
	set(Units_test_inc "${gtest_SOURCE_DIR}/include")
	set(Units_test_lib gtest_main)
endmacro()
