cmake_minimum_required(VERSION 3.0)

macro(SetupEvniroment)
	if(MSVC)
		# enable filters
		set_property(GLOBAL PROPERTY USE_FOLDERS ON)
		# Use dynamic libs
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}	/MT /Z7")
		set(CMAKE_CXX_FLAGS_DEBUG	"${CMAKE_CXX_FLAGS_DEBUG}   /MTd /Ot")
		# Multiprocessor support
		set(CMAKE_CXX_FLAGS			"${CMAKE_CXX_FLAGS}			/MP")
	endif()

	# policy warning fix...
	if (POLICY CMP0074)
		cmake_policy(SET CMP0074 NEW)
	endif()
endmacro()
