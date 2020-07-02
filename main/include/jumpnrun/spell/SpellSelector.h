#pragma once
#include"jumpnrun/system/Widget.h"
//#include"Media.h"
#include"framework/VirtualController.h"
#include"jumpnrun/spell/Spell.h"
#include"jumpnrun/spell/Deck.h"
//#include"Widget.h"

namespace jnr
{
	class SpellSelector/*: public Widget*/
	{
	public:
		SpellSelector(gfw::VirtualController& vc, Deck* deck);
		~SpellSelector();
		void run();
		void render(gfw::BaseMedia& media);
		bool loadDeck();
		Deck& getDeck(){ return *deck; }
		void setDeck(Deck* deck){ this->deck = deck; }
		void SetCursor(int i){ cursor = i; }
		int getCursor(){ return cursor; }
		static const int num_handcards = 5;

	private:
		int age;
		int num_rest_cards;
		int cursor;
		Deck* deck;
		gfw::VirtualController& vc;
		void renderNum(gfw::BaseMedia& media, int x, int y, int n);
	};
}