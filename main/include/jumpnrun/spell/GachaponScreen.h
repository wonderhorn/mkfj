#pragma once
#include"framework/Screen.h"
#include"jumpnrun/spell/Deck.h"
#include"framework/Task.h"
#include"framework/Object.h"
#include"jumpnrun/stage/Stage.h"
#include"framework/GameUtil.h"
#include"jumpnrun/system/ObjectManager.h"
#include"jumpnrun/ai/AI_Random.h"
#include"jumpnrun/system/JumpnRunData.h"
#include"jumpnrun/spell/DeckEditScreen.h"
#include<vector>
#include<tuple>
namespace jnr
{
	namespace spl
	{
		class GachaponScreen : public gfw::Screen
		{
		public:
			GachaponScreen(gfw::Game* game) : gfw::Screen(game){}
			bool initialize();
			void finalize();
			eScreenState run(gfw::BaseMedia& media);
			void render(gfw::BaseMedia& media);
		private:
			void gacha();

			enum eGachaponScreenStates {SelectCards, ConfirmGacha };
			jnr::JumpnRunData data;
			int cursor, cursor_tab, scroll, age, counter_cursor, confirm_cursor;
			eGachaponScreenStates state; //0: select cards, 1:
			std::vector<DeckTab> tabs;
			DeckTab all_cards, cards_in_deck;
			gfw::Timer timer_button;
			gfw::Tasklist tl;
			gfw::Object::Pipe player;
			jnr::Stage stg;
			gfw::ObjectManager om;
			jnr::ai::AI* ai;
			int prev_camera_x, prev_camera_y;
			std::vector<int> selected_card;
		};
	};
};