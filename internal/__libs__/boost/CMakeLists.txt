add_definitions( -DBOOST_ALL_DYN_LINK )
set(Boost_USE_STATIC_RUNTIME on)
set(Boost_USE_STATIC_LIBS    on)
set(Boost_USE_MULTITHREADED  on)
find_package(Boost REQUIRED 
    filesystem 
    date_time 
    thread
    system
    regex)

Unit(boost)
set(bFlat on)
set(Includes	${Boost_INCLUDE_DIR})
set(Libs        ${Boost_LIBRARIES})
HeaderLibrary()
