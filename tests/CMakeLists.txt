cmake_minimum_required(VERSION 3.1)
project(VoronoiTest)


# Build UnitTest++
add_subdirectory(unittest-cpp)


# Build Voronoi library
file(GLOB headersVoronoi_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../src/*.h)
file(GLOB sourcesVoronoi_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../src/*.cpp)
source_group("Voronoi" FILES ${headersVoronoi_} ${sourcesVoronoi_})
add_library(Voronoi STATIC ${headersVoronoi_} ${sourcesVoronoi_})
set_property(TARGET Voronoi PROPERTY CXX_STANDARD 11)


# Build the test runner for Voronoi
file(GLOB VORONOI_TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp src/*.h)
source_group("VoronoiTest" FILES ${VORONOI_TEST_SRCS})
include_directories(../src unittest-cpp/UnitTest++)
add_executable(VoronoiTest ${VORONOI_TEST_SRCS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
target_link_libraries(VoronoiTest UnitTest++ Voronoi)


## Run unit tests as post build step
add_custom_command(TARGET VoronoiTest
        POST_BUILD COMMAND VoronoiTest
        COMMENT "Testing Voronoi library")

