if (NOT MSVC)
    set(Libs stdc++fs)
    endif()

GB_Module( server
    Libs ${Libs}
    Modules
        configs 
        boost
        date
        mmap)
