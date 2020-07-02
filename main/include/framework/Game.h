#pragma once
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include"framework/Screen.h"
#include"framework/MediaAlternative.h"

#include<deque>
namespace gfw
{
	class Screen;
	class Game
	{
	public:
		enum eGameState{ Finished, Continuing };
		Game();
		bool initialize();
		void finalize();
		eGameState run(BaseMedia& media);
		void render(BaseMedia& media);

		void push_front(gfw::Screen* scr){ screens.push_front(scr); }
		void push_back(gfw::Screen* scr){ screens.push_back(scr); }

	private:
		std::deque<gfw::Screen*> screens;
		bool initialized;
		int age;
		//Media media;
	};
}