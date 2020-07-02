#pragma once
#include"framework/GameUtil.h"
#include"framework/BaseMedia.h"
#include"framework/Screen.h"

namespace jnr
{
	namespace spl
	{
		class CardGalleryScreen : public gfw::Screen
		{
		public:
			CardGalleryScreen(gfw::Game* game);
			bool initialize();
			void finalize();
			eScreenState run(gfw::BaseMedia& media);
			void render(gfw::BaseMedia& media);

		private:
			int cursor_x, cursor_y;
			int age, counter_cursor;
			gfw::Timer timer_button;
		};
	};
};
