#pragma once
#include"framework/Game.h"
#include"framework/BaseMedia.h"

namespace gfw
{
	class Game;
	class Screen
	{
	public:
		Screen() : game(NULL){}
		Screen(gfw::Game* game) : game(game){}
		enum eScreenState{ Finished, Continuing };

		virtual bool initialize() = 0;
		virtual void finalize() = 0;
		virtual eScreenState run(gfw::BaseMedia& media) = 0;
		virtual void render(gfw::BaseMedia& media) = 0;

	protected:
		gfw::Game* game;
	};
}