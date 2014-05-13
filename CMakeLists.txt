# Top-level cmake file. All settings are inherited by children cmake files.

# Recursively includes all children cmake files using add_subdirectory(...)

cmake_minimum_required( VERSION 2.6 )

if ($ENV{BUILD_OPTIMIZE})
    message("Building release... no debug symbols will be available.")
    set(CMAKE_BUILD_TYPE Release)
else()
    message("Building debug... performance will not be optimal.")
    set(CMAKE_BUILD_TYPE Debug)
endif()

if ($ENV{BUILD_FLAGS})
    add_definitions ($ENV{BUILD_FLAGS})
endif()

#if ($ENV{BUILD_FOLLOW_SET_AS_VECTOR})
#    add_definitions(-DVECTOR_FOLLOW_SET_THRESHOLD=2147483647)
#else()
#    # From the statistic '95% of users have <100 followers'
#    # Also, a decent (small) threshold for linear access.
#    # After this threshold, Google's sparse-hash is used.
#    add_definitions(-DVECTOR_FOLLOW_SET_THRESHOLD=100)
#endif()

    add_definitions(-DVECTOR_FOLLOW_SET_THRESHOLD=2147483647)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(APPLE) 
    elseif ($ENV{BUILD_SANITIZE})
        SET (CMAKE_EXE_LINKER_FLAGS
            -fsanitize=address 
        )

        add_definitions(
           -fsanitize=address 
        )
    endif()
else()
    add_definitions(-DNDEBUG)
    if ($ENV{BUILD_PROF_GEN})
        SET (CMAKE_EXE_LINKER_FLAGS
           -fprofile-generate
        )
        add_definitions(
           -fprofile-generate
        )
    elseif( $ENV{BUILD_PROF_USE})
        SET (CMAKE_EXE_LINKER_FLAGS
           -fprofile-use
        )
        add_definitions(
           -fprofile-use
        )
    endif()
endif()

add_definitions(-pthread)

if(APPLE)
   set(UV_LIBS m "-framework CoreFoundation" "-framework CoreServices")
elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
   set(UV_LIBS m pthread)
else()
   set(UV_LIBS rt m pthread)
endif()

add_subdirectory(src)
