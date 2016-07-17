# Compile everything under 'dependencies'
add_subdirectory(dependencies)

# Add include directories
include_directories(
	"." 
	dependencies
	dependencies/yaml-cpp/include
        dependencies/luawrap-lib/include/
	dependencies/lua/
	dependencies/boost_1_58_0/
)

add_definitions(-Wno-sign-compare -Wno-reorder -Wall -Wno-unused-variable  -Wuninitialized -Wno-deprecated-declarations -fPIC -std=c++11)
#add_definitions(-DREFACTORING)
#add_definitions(-DREFACTORING_DEBUG_OUTPUT)

# Find source files
aux_source_directory(
	"." 
	networksim_src
)

aux_source_directory(
	"unit_tests" 
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

#set_source_files_properties(
    #        ${networksim_src}
        #        PROPERTIES COMPILE_FLAGS 
        #        "-Wformat=0 -pedantic-errors -Wno-long-long -Wno-variadic-macros -Wno-sign-compare -Wno-reorder"
        #)

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
	hashkat
	${mersenne_simd_src} 
	${networksim_src} 
	${networksim_tests} 
	${lcommon_src} 
	${dependencies_src}
)

add_library(hashkat-lib
        SHARED
	${mersenne_simd_src} 
	${networksim_src} 
	${networksim_tests} 
	${lcommon_src} 
	${dependencies_src}
)

target_link_libraries (
	hashkat 
	UnitTest++
	yaml-cpp
        lualinenoise
        lua
        luawrap
        ${UV_LIBS}
)

target_link_libraries (
	hashkat-lib
	UnitTest++
	yaml-cpp
        lualinenoise
        lua
        luawrap
        ${UV_LIBS}
)

# installing through cmake . && make && sudo make install
#install (TARGETS hashkat DESTINATION bin)
#execute_process(COMMAND cp hashkat_pre.py /usr/local/bin)
