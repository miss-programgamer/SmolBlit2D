# General project settings
cmake_minimum_required(VERSION 3.29.0)
project(SmolBlit2D VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# Define cache variables
set(SMOL_BLIT2D_BUILD_TESTS FALSE CACHE BOOL "Whether unit tests should be built")
set(SMOL_BLIT2D_BUILD_EXAMPLES FALSE CACHE BOOL "Whether examples should be built")

# Define list of project source files
set(SMOL_BLIT2D_SOURCES
	"./source/smol/blit2d/vec2.hpp"
	"./source/smol/blit2d/size.hpp"
	"./source/smol/blit2d/rect.hpp"
	"./source/smol/blit2d/color.hpp"
	"./source/smol/blit2d/color.cpp"
	"./source/smol/blit2d/palette.hpp"
	"./source/smol/blit2d/pal_bitmap.hpp"
	"./source/smol/blit2d/pal_bitmap.cpp"
	"./source/smol/blit2d/bitmap.hpp"
	"./source/smol/blit2d/bitmap.cpp"
	"./source/smol/blit2d/tileset.hpp"
	"./source/smol/blit2d/tileset.cpp"
	"./source/smol/blit2d/tilemap.hpp"
	"./source/smol/blit2d/tilemap.cpp"
	"./source/smol/blit2d/renderer.hpp"
	"./source/smol/blit2d/renderer.cpp"
	"./source/smol/blit2d.hpp"
)

# Add main library target
add_library(SmolBlit2D STATIC ${SMOL_BLIT2D_SOURCES})
target_include_directories(SmolBlit2D PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>")

# Add example targets
if (SMOL_BLIT2D_BUILD_EXAMPLES AND MSVC)
	add_library(SmolBlit2DExampleApp STATIC "examples/example_app.hpp" "examples/example_app.cpp")
	target_link_libraries(SmolBlit2DExampleApp d2d1 dwrite SmolBlit2D)
	target_compile_definitions(SmolBlit2DExampleApp PUBLIC UNICODE _UNICODE)
	
	add_executable(SmolBlit2DExample1 WIN32 "examples/example1.cpp")
	target_link_libraries(SmolBlit2DExample1 SmolBlit2DExampleApp SmolBlit2D)
endif()

# Enable testing
if(SMOL_BLIT2D_BUILD_TESTS)
	find_package(Catch2 3 REQUIRED)
	add_executable(SmolBlit2DTests "tests/tests.cpp")
	target_link_libraries(SmolBlit2DTests PRIVATE Catch2::Catch2WithMain SmolBlit2D)
	include(CTest)
	include(Catch)
	catch_discover_tests(SmolBlit2DTests)
	enable_testing()
endif()

# Project packaging settings
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)