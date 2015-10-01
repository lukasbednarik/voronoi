cmake_minimum_required(VERSION 2.8.1)
project(VoronoiTest)

# Create UnitTest++ library
## Get the main sources
file(GLOB headers_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} unittest-cpp/UnitTest++/*.h)
file(GLOB sources_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} unittest-cpp/UnitTest++/*.cpp)
source_group("" FILES ${headers_} ${sources_})

## Get platform specific sources
if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    set(platformDir_ Win32)
else()
    set(platformDir_ Posix)
endif(WIN32)

file(GLOB platformHeaders_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} unittest-cpp/UnitTest++/${platformDir_}/*.h)
file(GLOB platformSources_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} unittest-cpp/UnitTest++/${platformDir_}/*.cpp)
source_group(${platformDir_} FILES ${platformHeaders_} ${platformSources_})

## Create the lib
add_library(UnitTest++ STATIC ${headers_} ${sources_} ${platformHeaders_} ${platformSources_})

# Build test runner for UnitTest++
file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} unittest-cpp/tests/*.cpp unittest-cpp/tests/*.h)
source_group( "" FILES ${TEST_SRCS})
add_executable(TestUnitTest++ ${TEST_SRCS})
include_directories(unittest-cpp)

target_link_libraries(TestUnitTest++ UnitTest++)

## Run unit tests as post build step
add_custom_command(TARGET TestUnitTest++
	POST_BUILD COMMAND TestUnitTest++
	COMMENT "Testing UnitTest++ library")

# Build the voronoi lib

set(CMAKE_CXX_FLAGS "-std=c++0x")

## Get the main sources
file(GLOB headersVoronoi_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../src/*.h)
file(GLOB sourcesVoronoi_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../src/*.cpp)
source_group("" FILES ${headersVoronoi_} ${sourcesVoronoi_})

## Create the lib
add_library(Voronoi STATIC ${headersVoronoi_} ${sourcesVoronoi_})


# Build the test runner for Voronoi

file(GLOB VORONOI_TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp src/*.h)
source_group( "" FILES ${VORONOI_TEST_SRCS})
add_executable(VoronoiTest ${VORONOI_TEST_SRCS})
include_directories(unittest-cpp)

target_link_libraries(VoronoiTest UnitTest++ Voronoi)

## Run unit tests as post build step
add_custom_command(TARGET VoronoiTest
        POST_BUILD COMMAND VoronoiTest
        COMMENT "Testing Voronoi library")
