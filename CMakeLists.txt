# Step 1: Build
cmake_minimum_required(VERSION 3.10)
project(feboundary VERSION 1.0.0 LANGUAGES CXX)
include(GNUInstallDirs)
add_library(${PROJECT_NAME} INTERFACE)
set(CMAKE_CXX_STANDARD 17)

target_include_directories(
	${PROJECT_NAME}
	INTERFACE
	$<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	)

# Part 2: Tests
find_package(Catch2 REQUIRED)

# Tests, don't remove commented executables
#add_executable(feboundary_test test/test.cpp)
#add_executable(feboundary_test test/struct_comparators.cpp)
#add_executable(feboundary_test test/test_ntop_x_size.cpp)
#add_executable(feboundary_test test/test_parser.cpp)
#add_executable(feboundary_test test/test_parser_4.cpp)
add_executable(feboundary_test test/sort.cpp)


target_link_libraries(
	feboundary_test PRIVATE Catch2::Catch2WithMain
	${PROJECT_NAME} stdc++fs
	)
target_include_directories(feboundary_test PRIVATE
	${PROJECT_SOURCE_DIR}/include
	)
include(CTest)
include(Catch)
catch_discover_tests(feboundary_test)
enable_testing()

