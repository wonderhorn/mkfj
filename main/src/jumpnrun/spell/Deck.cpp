#include"jumpnrun/spell/Deck.h"
#include"utils/string_util.h"
#include <algorithm>
#include<cstdlib>
using namespace jnr;
using namespace spl;
/*
const std::vector<int> Deck::initial_deck =
{
	spl::spellname2id("explosion"),
	spl::spellname2id("explosion"),
	spl::spellname2id("explosion"),
	spl::spellname2id("bullet"),
	spl::spellname2id("bullet"),
	spl::spellname2id("bullet"),
	spl::spellname2id("frog"),
	spl::spellname2id("frog"),
	spl::spellname2id("starshoot"),
	spl::spellname2id("starshoot"),
	spl::spellname2id("starshoot"),
	spl::spellname2id("lifegain20"),
	spl::spellname2id("lifegain20"),
};*/

Card::Card() : value(-1), used(false), spell(){}

Deck::Deck() : current_card(0)
{
	auto deck_list =
	{
		spl::spellname2id("explosion"),
		spl::spellname2id("explosion"),
		spl::spellname2id("explosion"),
		spl::spellname2id("bullet"),
		spl::spellname2id("bullet"),
		spl::spellname2id("bullet"),
		spl::spellname2id("frog"),
		spl::spellname2id("frog"),
		spl::spellname2id("starshoot"),
		spl::spellname2id("starshoot"),
		spl::spellname2id("starshoot"),
		spl::spellname2id("lifegain20"),
		spl::spellname2id("lifegain20"),
	};
	/*for (int i = 0; i < Spell::NUM_SPELL * 4; i++)
	{
		draw_counter.push_back(0);
		Card c;
		c.value = i % Spell::NUM_SPELL;
		c.spell = Spell::spells[c.value];
		cards.push_back(c);
	}*/
	for (int id : deck_list)
	{
		draw_counter.push_back(0);
		Card c;
		c.value = id % Spell::NUM_SPELL;
		c.spell = Spell::spells[c.value];
		cards.push_back(c);
	}
	rest_num = cards.size();
}

void Deck::initialize()
{
	auto deck_list =
	{
		//spl::spellname2id("explosion"),
		//spl::spellname2id("explosion"),
		//spl::spellname2id("explosion"),
		//spl::spellname2id("bullet"),
		spl::spellname2id("bullet"),
		spl::spellname2id("bullet"),
		spl::spellname2id("frog"),
		spl::spellname2id("frog"),
		spl::spellname2id("starshoot"),
		spl::spellname2id("starshoot"),
		spl::spellname2id("starshoot"),
		//spl::spellname2id("lifegain20"),
		//spl::spellname2id("lifegain20"),
	};
	draw_counter.clear();
	cards.clear();
	for (int id : deck_list)
	{
		draw_counter.push_back(0);
		Card c;
		c.value = id % Spell::NUM_SPELL;
		c.spell = Spell::spells[c.value];
		cards.push_back(c);
	}
	rest_num = cards.size();
}

void Deck::initialize(int num_cards)
{
	cards.clear();
	draw_counter.clear();
	cards.resize(num_cards);
	draw_counter.resize(num_cards);
	for (int i = 0; i < cards.size(); i++)
	{
		draw_counter[i] = 0;
	}
}

void Deck::initialize(const std::vector<std::string>& card_ids)
{
	cards.clear();
	draw_counter.clear();
	const int nc = card_ids.size() > NumHandCard() ? card_ids.size() : NumHandCard();
	//const int nc = card_ids.size();
	cards.resize(nc);
	draw_counter.resize(nc);
	rest_num = 0;
	for (int i = 0; i < card_ids.size(); i++)
	{
		if (card_ids[i].length() <= 0) {
			int id = -1;
			cards[i].value = id;
			cards[i].spell = jnr::spl::Spell::spells[0];
			cards[i].used = true;
			continue;
		}

		int id = myutil::str2int(card_ids[i]);
		if (id >= 0)
		{
			cards[i].value = id;
			cards[i].spell = Spell::spells[cards[i].value];
			cards[i].used = 0;
			rest_num++;
		}
		else
		{
			cards[i].value = id;
			cards[i].spell = Spell::spells[0];
			cards[i].used = 1;
		}
	}
	//dummies
	for (int i = card_ids.size(); i < this->NumHandCard(); i++)
	{
		int id = -1;
		cards[i].value = id;
		cards[i].spell = jnr::spl::Spell::spells[0];
		cards[i].used = true;
	}

	for (int i = 0; i < cards.size(); i++)
	{
		int id = cards[i].value;
		if (id >= 0)
		{
			draw_counter[i] = 00;
		}
		else
		{
			draw_counter[i] = -1;
		}
		draw_counter[i] = 00;
	}
	//dummies
	for (int i = card_ids.size(); i < this->NumHandCard(); i++)
	{
		draw_counter[i] = -1;
	}
}

void Deck::reset()
{
	rest_num = 0;
	for (int i = 0; i < cards.size(); i++)
	{
		draw_counter[i] = 0;
		if(cards[i].value < 0)
			cards[i].used = 1;
		else
		{
			cards[i].spell = Spell::spells[cards[i].value];
			cards[i].used = 0;
			rest_num++;
		}
	}
	for (int i = 0; i < cards.size(); i++)
	{
		draw_counter[i] = 90;
	}

	// send cards whose value == -1 to the end of the vector
	std::sort(cards.begin(), cards.end(), [](auto const& a, auto const& b) {

		return a.value > b.value;
	});
	this->shuffle();
}

void Deck::shuffle()
{
	if (cards.size() == 0)
		return;
	for (int i = 0; i < 1000; i++)
	{
		int idx1 = rand() % cards.size();
		int idx2 = rand() % cards.size();
		if(cards[idx1].value >= 0 && cards[idx2].value >= 0)
			swap_cards(idx1, idx2);
	}
}

void Deck::swap_cards(int Card1, int Card2)
{
	Card temp = cards[Card1];
	cards[Card1] = cards[Card2];
	cards[Card2] = temp;
}

static int countValidCards(const std::vector<jnr::Card> cards) 
{
	int count = 0;
	for (const auto& c : cards)
		count += (c.value >= 0);
	return count;
}

Spell Deck::use(int Card)
{
	if (cards[Card].used == true)
		return Spell();
	cards[Card].used = true;
	Spell rtn = cards[Card].spell;
	/*for(int i = Card; i < NUM_CARDS - 1; i++)
	{
	swap_cards(i, i + 1);
	}*/
	rest_num--;

	const int NUM_HANDCard = NumHandCard();
	const int num_cards = countValidCards(cards);
	if (NUM_HANDCard < cards.size())
	{
		int next_idx = NUM_HANDCard > num_cards ? num_cards : NUM_HANDCard;
		swap_cards(Card, next_idx);
		for (int j = next_idx; j < cards.size() - 1; j++)
		{
			swap_cards(j, j + 1);

			int temp = draw_counter[j];
			draw_counter[j] = draw_counter[j + 1];
			draw_counter[j + 1] = temp;
		}
	}
	draw_counter[Card] = 120;

	return rtn;
}

void Deck::run()
{
	const int NUM_HANDCard = NumHandCard();
	for (int i = 0; i < cards.size(); i++)
	{
		if (draw_counter[i])
			draw_counter[i]--;
		/*
		if (draw_counter[i] == 1)
		{
			if (NUM_HANDCard < cards.size())
			{
				int next_idx = NUM_HANDCard > cards.size() - 1 ? cards.size() - 1 : NUM_HANDCard;
				swap_cards(i, next_idx);
				for (int j = next_idx; j < cards.size() - 1; j++)
				{
					swap_cards(j, j + 1);

					int temp = draw_counter[j];
					draw_counter[j] = draw_counter[j + 1];
					draw_counter[j + 1] = temp;
				}
			}
		}*/
	}
}

int Deck::numValidCards()const
{
	int num_valid_card = 0;
	for (int i = 0; i < cards.size(); i++)
	{
		if (cards[i].value >= 0)
			num_valid_card++;
	}
	return num_valid_card;
}

bool Deck::finished() const
{
	int num_valid_card = 0;
	for (int i = 0; i < cards.size(); i++)
	{
		if (!cards[i].used)
			return false;
		if (cards[i].value >= 0)
			num_valid_card++;
	}
	//return num_valid_card > 0;
	return true;
}