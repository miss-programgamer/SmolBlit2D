#include "input.hpp"


#define CYCLE(btn) btn = (btn << 1) | (btn & 0b1)


void Smol::Blit2D::Input::Cycle()
{
	CYCLE(mouse_l_btn);
	CYCLE(mouse_m_btn);
	CYCLE(mouse_r_btn);
	CYCLE(pad_n_btn);
	CYCLE(pad_s_btn);
	CYCLE(pad_w_btn);
	CYCLE(pad_e_btn);
	CYCLE(face_n_btn);
	CYCLE(face_s_btn);
	CYCLE(face_w_btn);
	CYCLE(face_e_btn);
	CYCLE(shld_l_btn);
	CYCLE(shld_r_btn);
}