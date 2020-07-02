#pragma once
#include"jumpnrun/spell/Spell.h"
namespace jnr
{
	class Animation
	{
	public:
		Animation();
		void reset(int refx, int refy, int interval
			, int num_frame, int w, int h, int last_frame_time = 0, int num_iteration = 1);
		int refx, refy, interval, num_frame, w, h, last_frame_time, num_iteration;
		bool finished;
		int counter;
		jnr::spl::eSpell spell;
		int spell_time;
		int RefX();
		int Frame();
		int Duration();
		int RefY();
		void Advance();
		void setSpell(jnr::spl::eSpell spell, int spell_time);
	};
};