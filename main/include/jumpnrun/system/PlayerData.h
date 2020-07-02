#pragma once
#include<string>
#include"framework/VirtualController.h"
#include"jumpnrun/spell/Deck.h"
namespace jnr
{
	class PlayerData
	{
	public:
		PlayerData(): vc(nullptr){}
		PlayerData(int id, std::string name, gfw::VirtualController* vc
			, Deck deck, bool isActive): id(id), name(name), vc(vc)
			, deck(deck), isActive(isActive), hp_max(50), hp_current(50), mp_max(50), mp_current(50)
			, score(0), max_score(0)
			, timer_deckreload(300){}
		int id;
		std::string name;
		gfw::VirtualController* vc;
		Deck deck, deck_tmp;
		bool isActive;
		int hp_max, hp_current, mp_max, mp_current, score, max_score;
		int timer_deckreload;

		void initialize(){}
		PlayerData& operator = (const PlayerData& p)
		{
			this->id = p.id;
			this->name = p.name;
			this->vc = p.vc;
			this->deck = p.deck;
			this->isActive = p.isActive;
			this->hp_max = p.hp_max;
			this->hp_current = p.hp_current;
			this->mp_max = p.mp_max;
			this->mp_current = p.mp_current;
			this->score = p.score;
			this->max_score = p.max_score;
			return *this;
		}
	};
};