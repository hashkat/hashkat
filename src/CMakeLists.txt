# Compile everything under 'dependencies'
add_subdirectory(dependencies)

add_definitions(-Wuninitialized -std=c++98)
#SET (CMAKE_EXE_LINKER_FLAGS -fsanitize=address)
#SET (CMAKE_MODULE_LINKER_FLAGS -fsanitize=address)

# Find source files
aux_source_directory(
	"." 
	networksim_src
)

aux_source_directory(
	"tests" 
	networksim_tests
)

aux_source_directory(
	"dependencies" 
	dependencies_src
)

# Workaround for problem with compiling C with -std=c++98:
set_source_files_properties(
	dependencies/ini.c 
	PROPERTIES LANGUAGE CXX
)

set_source_files_properties(
        ${networksim_src}
        PROPERTIES COMPILE_FLAGS 
        "-pedantic-errors -Wno-long-long"
)


aux_source_directory(
	"dependencies/lcommon" 
	lcommon_src
)

# Add include directories

include_directories(".")

# Add compilation target
add_executable( 
	main 
	${networksim_src} 
	${networksim_tests} 
	${lcommon_src} 
	${dependencies_src}
)

target_link_libraries (
	main 
	UnitTest++
	yaml-cpp
)
