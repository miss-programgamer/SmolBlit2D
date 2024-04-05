#ifndef SMOL_CORE2D_VEC2_DEFINED
#define SMOL_CORE2D_VEC2_DEFINED


namespace Smol
{
	// A 2D vector represented by components on two axes.
	template<typename T>
	struct Vec2
	{
		T x;
		T y;
	};
	
	
	using Vec2F = Vec2<float>;
	using Vec2I = Vec2<int>;
}


#endif // SMOL_CORE2D_VEC2_DEFINED