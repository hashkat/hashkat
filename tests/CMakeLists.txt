add_definitions(-std=c++11)

include_directories(
../src/dependencies/boost_1_58_0/
)

# Add compilation target
add_executable( 
test_hashkat
test.cpp
)
