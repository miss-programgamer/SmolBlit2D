#ifndef SMOL_BLIT2D_SIZE_DEFINED
#define SMOL_BLIT2D_SIZE_DEFINED


namespace Smol::Blit2D
{
	template<typename T>
	struct Size
	{
		T w;
		T h;
	};
	
	
	using SizeI = Size<int>;
	using SizeF = Size<float>;
}


#endif // SMOL_BLIT2D_SIZE_DEFINED