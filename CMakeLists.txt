cmake_minimum_required( VERSION 2.6 )
PROJECT(socialnodes)
add_definitions( -std=c++98 )

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

aux_source_directory("socialnodes" socialnodes_src) #Find source files
aux_source_directory("socialnodes/dependencies" dependencies_src) #Find source files

add_executable( main ${socialnodes_src} ${dependencies_src})
