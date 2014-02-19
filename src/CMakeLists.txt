# Compile everything under 'dependencies'
add_subdirectory(dependencies)

# Add include directories
include_directories(
	"." 
	dependencies
	dependencies/yaml-cpp/include
	dependencies/libuv/include
)


add_definitions(-Wuninitialized -std=c++98 -O0 -ggdb)

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
        "-pedantic-errors -Wno-long-long -Wno-variadic-macros -Wno-sign-compare -Wno-reorder"
        #"-Wall -Wextra -Werror -pedantic-errors -Wno-long-long -Wno-variadic-macros -Wno-sign-compare -Wno-reorder"
)


aux_source_directory(
	"dependencies/lcommon" 
	lcommon_src
)

aux_source_directory(
	"dependencies/mersenne-simd" 
	mersenne_simd_src
)

# Add compilation target
add_executable( 
	main
	${mersenne_simd_src} 
	${networksim_src} 
	${networksim_tests} 
	${lcommon_src} 
	${dependencies_src}
)

target_link_libraries (
	main 
	UnitTest++
	yaml-cpp
	libuv
        ${UV_LIBS}
)
