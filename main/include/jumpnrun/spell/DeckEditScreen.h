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
#include<vector>
#include<tuple>
namespace jnr
{
	namespace spl
	{
		class DeckTab
		{
		public:
			DeckTab();
			std::vector<std::tuple<int, int>> cards;
			std::string name;
			int numCards()const;
			Deck writeDeck()const;

			static DeckTab fromDeck(const Deck& deck);
			static DeckTab allCards();
			static DeckTab gachaInitial();
			static DeckTab cardsInBag();
		};
		class DeckEditScreen: public gfw::Screen
		{
		public:
			DeckEditScreen(gfw::Game* game) : gfw::Screen(game){}
			bool initialize();
			void finalize();
			eScreenState run(gfw::BaseMedia& media);
			void render(gfw::BaseMedia& media);
		private:
			jnr::JumpnRunData data;
			int cursor, cursor_tab, scroll, age, counter_cursor;
			std::vector<DeckTab> tabs;
			DeckTab all_cards;
			gfw::Timer timer_button;
			gfw::Tasklist tl;
			gfw::Object::Pipe player;
			jnr::Stage stg;
			gfw::ObjectManager om;
			jnr::ai::AI* ai;
			int prev_camera_x, prev_camera_y;
		};
	};
};