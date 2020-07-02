#pragma once
#include<vector>
#include"jumpnrun/spell/Spell.h"
namespace jnr
{
	class Card
	{
	public:
		Card();
		int value;
		spl::Spell spell;
		bool used;

		static Card Dummy(){ static Card dummy; dummy.used = true; return dummy; }
	};

	class Deck
	{
	public:
		Deck();
		int current_card;
		//static const int NUM_CARDS = 30;
		void shuffle();
		void swap_cards(int card1, int card2);
		spl::Spell use(int card);
		int NumRest()const { return rest_num; }
		void addNumRest(){ rest_num++; }
		void initialize();
		void initialize(int num_cards);
		void initialize(const std::vector<std::string>& card_ids);
		Card& getCard(int i){ if (i < cards.size())return cards[i]; else return dummy; }
		Card getCardConst(int i)const{ return cards[i]; }
		const std::vector<int>& DrawCounter()const{ return draw_counter; }
		void run();
		void reset();
		bool finished() const;
		int numCards()const { return cards.size(); }
		int numValidCards()const;
		Deck(const Deck& d)
		{
			this->current_card = d.current_card;
			this->rest_num = d.rest_num;
			//this->cards = d.cards;
			this->cards.clear();
			for (int i = 0; i < d.cards.size(); i++)
				this->cards.push_back(d.cards[i]);
			this->draw_counter.clear();
			for (int i = 0; i < d.draw_counter.size(); i++)
				this->draw_counter.push_back(d.draw_counter[i]);
			//this->draw_counter = d.draw_counter;
		}
		Deck& operator =(const Deck& d)
		{
			this->current_card = d.current_card;
			this->rest_num = d.rest_num;
			//this->cards = d.cards;
			this->cards.clear();
			for (int i = 0; i < d.cards.size(); i++)
				this->cards.push_back(d.cards[i]);
			this->draw_counter.clear();
			for (int i = 0; i < d.draw_counter.size(); i++)
				this->draw_counter.push_back(d.draw_counter[i]);
			//this->draw_counter = d.draw_counter;
			return *this;
		}
		void setDrawCounter(int idx, int v)
		{
			if (draw_counter.size() <= idx)
			{
				for (int i = draw_counter.size(); i <= idx; i++)
					draw_counter.push_back(-1);
			}
			draw_counter[idx] = v;
		}
		int NumHandCard() { return 5; }

	private:
		//Card cards[NUM_CARDS];
		Card dummy;
		std::vector<Card> cards;
		std::vector<int> draw_counter;
		//int draw_counter[NUM_CARDS];
		int rest_num;
		static const std::vector<int> initial_deck;
	};
};