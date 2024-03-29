#ifndef SMOL_BLIT2D_VEC2_DEFINED
#define SMOL_BLIT2D_VEC2_DEFINED


namespace Smol::Blit2D
{
	template<typename T>
	struct Vec2
	{
		T x;
		T y;
	};
	
	using Vec2F = Vec2<float>;
	using Vec2I = Vec2<int>;
}


#endif // SMOL_BLIT2D_VEC2_DEFINED