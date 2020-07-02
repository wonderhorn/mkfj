#pragma once
//#include"AI.h"
#include"jumpnrun/mover/character/Character.h"
#include<vector>
namespace jnr
{
	namespace ai
	{
		class Radar
		{
		public:
			Radar();
			std::vector<jnr::Mover*> scan(double R);
			gfw::Tasklist* tl;
			int num_bin;
			int myid;
			bool turned;
			gmtr::Vector2D myp;
		};
	};
};