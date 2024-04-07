# General project settings
cmake_minimum_required(VERSION 3.29.0)
project(SmolBlit2D VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# Define cache variables
set(SMOL_BLIT2D_BUILD_TESTS FALSE CACHE BOOL "Whether unit tests should be built")
set(SMOL_BLIT2D_BUILD_EXAMPLES FALSE CACHE BOOL "Whether examples should be built")

# Find required packages
find_package(SmolCore2D REQUIRED)

# Define list of project source files
set(SMOL_BLIT2D_HEADERS
	"./source/smol/blit2d/color.hpp"
	"./source/smol/blit2d/palette.hpp"
	"./source/smol/blit2d/pal_bitmap.hpp"
	"./source/smol/blit2d/bitmap.hpp"
	"./source/smol/blit2d/tileset.hpp"
	"./source/smol/blit2d/tilemap.hpp"
	"./source/smol/blit2d/renderer.hpp"
	"./source/smol/blit2d.hpp"
)

set(SMOL_BLIT2D_SOURCES
	"./source/smol/blit2d/color.cpp"
	"./source/smol/blit2d/pal_bitmap.cpp"
	"./source/smol/blit2d/bitmap.cpp"
	"./source/smol/blit2d/tileset.cpp"
	"./source/smol/blit2d/tilemap.cpp"
	"./source/smol/blit2d/renderer.cpp"
)

# Add main library target
add_library(SmolBlit2D STATIC ${SMOL_BLIT2D_SOURCES})
target_sources(SmolBlit2D PUBLIC FILE_SET HEADERS BASE_DIRS "source" FILES ${SMOL_BLIT2D_HEADERS})
target_include_directories(SmolBlit2D PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>")
target_include_directories(SmolBlit2D PUBLIC "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries(SmolBlit2D PUBLIC Smol::SmolCore2D)

# Add example targets
if (SMOL_BLIT2D_BUILD_EXAMPLES AND MSVC)
	set(SMOL_BLIT2D_EXAMPLES_SOURCES
		"examples/input.hpp"
		"examples/input.cpp"
		"examples/example_app.hpp"
		"examples/example_app.cpp"
	)
	
	add_library(SmolBlit2DExampleApp STATIC ${SMOL_BLIT2D_EXAMPLES_SOURCES})
	target_link_libraries(SmolBlit2DExampleApp d2d1 dwrite SmolBlit2D)
	target_compile_definitions(SmolBlit2DExampleApp PUBLIC UNICODE _UNICODE)
	
	add_executable(SmolBlit2DExample1 WIN32 "examples/example1.cpp")
	target_link_libraries(SmolBlit2DExample1 SmolBlit2DExampleApp SmolBlit2D)
	
	add_executable(SmolBlit2DExample2 WIN32 "examples/example2.cpp")
	target_link_libraries(SmolBlit2DExample2 SmolBlit2DExampleApp SmolBlit2D)
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

# Generate CMake integration scripts
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

configure_package_config_file("cmake/Config.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/SmolBlit2DConfig.cmake"
	INSTALL_DESTINATION "lib/cmake/SmolBlit2D"
)

write_basic_package_version_file(SmolBlit2DConfigVersion.cmake
	VERSION ${CMAKE_PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

# Install targets & scripts
install(TARGETS SmolBlit2D EXPORT SmolBlit2DTargets LIBRARY DESTINATION ${CMAKE_CURRENT_BINARY_DIR} FILE_SET HEADERS)
install(EXPORT SmolBlit2DTargets FILE SmolBlit2DTargets.cmake NAMESPACE Smol:: DESTINATION "lib/cmake/SmolBlit2D")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SmolBlit2DConfigVersion.cmake" DESTINATION "lib/cmake/SmolBlit2D")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SmolBlit2DConfig.cmake" DESTINATION "lib/cmake/SmolBlit2D")