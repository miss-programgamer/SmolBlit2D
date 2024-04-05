#include <catch2/catch_test_macros.hpp>
#include <smol/core2d.hpp>
using namespace Smol;


TEST_CASE("Rect/Hit Test Vs. Points", "[rect]")
{
	RectF r1{ 0, 0, 5, 5 };
	
	REQUIRE(r1.Contains({ 2, 3 }));
	REQUIRE_FALSE(r1.Contains({ 6, 3 }));
	REQUIRE_FALSE(r1.Contains({ 3, -2 }));
}


TEST_CASE("Rect/Hit Test Vs. Other Rects", "[rect]")
{
	RectF r1{ 0, 0, 5, 5 };
	
	REQUIRE(r1.Overlaps({ 1, 1, 4, 4 }));
	REQUIRE_FALSE(r1.Overlaps({ -1, -3, 0, 0 }));
	REQUIRE_FALSE(r1.Overlaps({ 5, 3, 7, 8 }));
}