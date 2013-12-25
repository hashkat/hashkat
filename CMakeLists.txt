# Top-level cmake file. All settings are inherited by children cmake files.
# Recursively includes all children cmake files using add_subdirectory(...)

cmake_minimum_required( VERSION 2.6 )

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

SET (CMAKE_EXE_LINKER_FLAGS
#    -fsanitize=address
)

#add_definitions( -std=c++98 -fsanitize=address)

add_subdirectory(src)
