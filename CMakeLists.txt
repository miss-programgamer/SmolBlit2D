cmake_minimum_required(VERSION 3.29.0)
project(SmolBlit2D VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

set(SMOL_BLIT2D_SOURCE
	"./source/smol/blit2d/vec2.hpp"
	"./source/smol/blit2d/rect.hpp"
	"./source/smol/blit2d/color.hpp"
	"./source/smol/blit2d/palette.hpp"
	"./source/smol/blit2d/pal_bitmap.hpp"
	"./source/smol/blit2d/pal_bitmap.cpp"
	"./source/smol/blit2d/bitmap.hpp"
	"./source/smol/blit2d/bitmap.cpp"
	"./source/smol/blit2d/renderer.hpp"
	"./source/smol/blit2d/renderer.cpp"
	"./source/smol/blit2d.hpp"
)

add_library(SmolBlit2D STATIC ${SMOL_BLIT2D_SOURCE})
target_include_directories(SmolBlit2D PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>")

if (MSVC)
	add_library(SmolBlit2DExampleApp STATIC "./examples/example_app.hpp" "./examples/example_app.cpp")
	target_link_libraries(SmolBlit2DExampleApp d2d1 dwrite SmolBlit2D)
	target_compile_definitions(SmolBlit2DExampleApp PUBLIC UNICODE _UNICODE)
	
	add_executable(SmolBlit2DExample1 WIN32 "./examples/example1.cpp")
	target_link_libraries(SmolBlit2DExample1 SmolBlit2DExampleApp SmolBlit2D)
	# target_compile_definitions(SmolBlit2DExample1 PUBLIC UNICODE _UNICODE)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)