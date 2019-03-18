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
	else()
		set(CMAKE_CXX_FLAGS			"${CMAKE_CXX_FLAGS}			-lstdc++fs")
		endif()
	# asio c++17 warnings...
	add_compile_definitions(_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING)
endmacro()
