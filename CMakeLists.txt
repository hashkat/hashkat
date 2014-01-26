# Top-level cmake file. All settings are inherited by children cmake files.

# Recursively includes all children cmake files using add_subdirectory(...)

cmake_minimum_required( VERSION 2.6 )

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

if(APPLE)
    set(UV_LIBS m "-framework CoreFoundation" "-framework CoreServices")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
   if(CMAKE_BUILD_TYPE STREQUAL "Debug")
       SET (CMAKE_EXE_LINKER_FLAGS
           -fsanitize=address
       )

       add_definitions(
           -fsanitize=address
           -D_GLIBCXX_DEBUG
       )
   else()
       SET (CMAKE_EXE_LINKER_FLAGS
           #-fprofile-generate
           #-fprofile-use
       )

       add_definitions(
           #-fprofile-generate
           #-fprofile-use
           -DNDEBUG
       )
   endif()
   set(UV_LIBS rt m pthread)
elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
   set(UV_LIBS m pthread)
endif()
add_subdirectory(src)
