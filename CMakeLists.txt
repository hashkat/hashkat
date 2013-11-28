# Top-level cmake file. All settings are inherited by children cmake files.
# Recursively includes all children cmake files using add_subdirectory(...)

cmake_minimum_required( VERSION 2.6 )

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

add_definitions( -std=c++98)

add_subdirectory(src)
