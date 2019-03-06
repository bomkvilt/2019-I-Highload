# Cmake code style:
# private macro/function -  <function_name>
# public  macro/function -  <Function_Name>
# local   variable		 - _<variable_name>
# private variable		 -  <variable_name>
# public  variable		 -  <Variable_Name>
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# add unit's sources/headers to the project
#
macro(setup_source)
	# code directories
	if (bFlat)
		set(source_dir "")
		set(header_dir "")
		set(data_dir   "")
	else()
		set(source_dir "Private/")
		set(header_dir "Public/")
		set(data_dir   "Data/")
	endif()
	# find unit files
	file(GLOB_RECURSE  sources  "${source_dir}*")
	file(GLOB_RECURSE  headers  "${header_dir}*")
	# add the files into a unit's code tree
	source_group(TREE ${unit_root} FILES ${sources})
	source_group(TREE ${unit_root} FILES ${headers})
endmacro()

macro(setup_modules)
	list(APPEND modules "")
	# get depending modules from assigned units
	foreach(_module ${Modules})
		get_property(_modules GLOBAL PROPERTY ${_module}_MOD)
		list(APPEND modules ${_modules})
	endforeach()
	# combine with unit modules
	list(APPEND modules ${Modules})
	# remove duplicates
	list(REMOVE_DUPLICATES modules)
endmacro()


# add required includes (from other modules and 3rdParty)
#
macro(setup_includes)
	# get public includes from assigned units
	foreach(_module ${modules})
		get_property(_unit_dir GLOBAL PROPERTY ${_module}_DIR)
		list(APPEND _includes ${_unit_dir}) 
	endforeach()
	# combine unit includes
	list(APPEND _includes ${header_dir} ${PrvIncs} ${PubIncs})
	# add the includes into the unit
	include_directories(${_includes})
endmacro()

# link required libs
#
macro(setup_libs)
	# get public libs from assigned modules
	foreach(_module ${modules})
		get_property(_unit_lib GLOBAL PROPERTY ${_module}_LIB)
		list(APPEND _libs ${_unit_lib})
	endforeach()
	# combine unit links
	list(APPEND _libs ${Private_Libraryes} ${Libs})
	# add the libs into the unit
	target_link_libraries(${unitName} ${_libs})
endmacro()

# set unit's folder in .vs project
#
macro(setup_folder)
	set(_path "${unit_root}/..")
	file(RELATIVE_PATH _category "${ENGINE_ROOT}" "${_path}")
	SET_TARGET_PROPERTIES(${unitName} PROPERTIES FOLDER "${_category}")
endmacro()

# setup unit properties
#
macro(setup_properties)
	SET_TARGET_PROPERTIES(${unitName} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${ENGINE_ROOT})
endmacro()

# add module meta into a global space
#
macro(defineUnit)
	# absolute include paths
	get_filename_component(_header_dir "${header_dir}" ABSOLUTE)
	get_filename_component(_pubIncs    "${PubIncs}"    ABSOLUTE)
	# combine export states
	set(_public_dirs ${_header_dir} ${_pubIncs} )
	set(_public_libs ${unitName}    ${Libs}     )
	# set global properties
	set_property(GLOBAL PROPERTY ${unitName}_DIR ${_public_dirs})
	set_property(GLOBAL PROPERTY ${unitName}_LIB ${_public_libs})
	set_property(GLOBAL PROPERTY ${unitName}_MOD ${modules})
	# local properies
	set(unit_root ${PROJECT_SOURCE_DIR})
	set(CMAKE_CXX_STANDARD 14)
endmacro()

#
#
macro(setup_settings)
	file(RELATIVE_PATH _category "${ENGINE_ROOT}" "${unit_root}")
	add_compile_definitions(DATA_DIR="${_category}/${data_dir}")
endmacro()

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
macro(Init_solution _project_name)
	project(${_project_name})
	set(CMAKE_CXX_STANDARD 14)
	set(ENGINE_ROOT ${PROJECT_SOURCE_DIR})
endmacro()

# define project
#
macro(Init_project ProjectPath)
	SUBDIRS(${ProjectPath})
endmacro()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# define new unit (module / application)
#
macro(Unit _Name)
	#-------------------------------------------------# interface variables
	set(Modules)			# required modules
	set(PrvIncs)			# include files from 3rdParty
	set(PubIncs)			# include files from 3rdParty
	set(Libs)				# .lib 	  files from 3tdParty
	set(bFlat	off)		# disable public | private | data subdirs
	#-------------------------------------------------# internal variables
	set(unit_root)			# unit root directory
	set(header_dir)			# found includes for the unit
	set(source_dir)			# found sources  for the unit
	set(data_dir)           # folder with module resources
	set(headers)			# unit headers
	set(modules)			# all project modules = Modules + Modules' modules + ...
	set(sources)			# unit sources
	#-------------------------------------------------# unit defenition
	set(unitName ${_Name})	# unit name
	project(${unitName})	# create a cmake project
	defineUnit()			# add the unit's meta into a global space
endmacro()

# use the unit as an module
#
macro(Module)
	setup_modules()
	setup_source()
	setup_includes()
	setup_settings()
	add_library(${unitName} 
		${headers} 
		${sources}
	)
	setup_folder() 
	defineUnit()
endmacro()

# use the unit as an application
#
macro(Application)
	setup_modules()
	setup_source()
	setup_includes()
	setup_settings()
	add_executable(${unitName} 
		${headers} 
		${sources}
	)
	setup_properties()
	setup_libs()
	setup_folder()
	defineUnit()
endmacro()

# 
#
macro(CodeBase)
	setup_modules()
	setup_source()
	setup_includes()
	add_custom_target(${unitName} 
		SOURCES ${headers} 
		SOURCES ${sources}
	)
	setup_settings()
	setup_folder() 
	defineUnit()
endmacro()
