#ifndef SMOL_BLIT2D_EXAMPLES_INPUT_DEFINED
#define SMOL_BLIT2D_EXAMPLES_INPUT_DEFINED


#include <smol/core2d.hpp>


namespace Smol::Blit2D
{
	struct Input
	{
		Vec2I mouse_pos;
		
		unsigned char mouse_l_btn;
		
		unsigned char mouse_m_btn;
		
		unsigned char mouse_r_btn;
		
		unsigned char pad_n_btn;
		
		unsigned char pad_s_btn;
		
		unsigned char pad_w_btn;
		
		unsigned char pad_e_btn;
		
		unsigned char face_n_btn;
		
		unsigned char face_s_btn;
		
		unsigned char face_w_btn;
		
		unsigned char face_e_btn;
		
		unsigned char shld_l_btn;
		
		unsigned char shld_r_btn;
		
		
		// Shift button states to age them by a frame.
		void Cycle();
		
		
		// Check whether a button is held down.
		static constexpr bool IsBtnDown(unsigned char btn)
		{ return btn & 0b01; }
		
		// Check whether a button was pressed this frame.
		static constexpr bool IsBtnPressed(unsigned char btn)
		{ return (btn & 0b11) == 0b01; }
		
		// Check whether a button was released this frame.
		static constexpr bool IsBtnReleased(unsigned char btn)
		{ return (btn & 0b11) == 0b10; }
	};
}


#endif // SMOL_BLIT2D_EXAMPLES_INPUT_DEFINED