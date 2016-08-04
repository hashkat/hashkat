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

if (${CMAKE_SYSTEM_NAME} MATCHES "SunOS") 
    add_definitions(-Du_int16_t="unsigned short")
endif()

if ($ENV{BUILD_FLAGS})
    add_definitions ($ENV{BUILD_FLAGS})
endif()

if ($ENV{NO_WARNINGS})
    add_definitions ("-w")
endif()

# Work-around for Travis CI
add_definitions( "-DBOOST_NO_CXX11_ALLOCATOR" )

# If we're using GCC< use -fno-pretty-templates to generate more readable templated error messages.
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_definitions(-fno-pretty-templates)
endif()
add_definitions ("-DLUA_USE_POSIX" "-Wno-deprecated-declarations" "-DHASHKAT_VERSION=\"$ENV{HASHKAT_VERSION}\"") 

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DGLIBCXX_DEBUG)
    if ($ENV{BUILD_SANITIZE})
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
add_subdirectory(tests)
