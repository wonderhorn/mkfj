#include"jumpnrun/spell/SpellSelector.h"

using namespace gfw;
using namespace jnr;
using namespace spl;

SpellSelector::SpellSelector(gfw::VirtualController& vc, Deck* deck)
	:age(0), cursor(0), deck(deck), num_rest_cards(deck->numCards()), vc(vc)
{}

SpellSelector::~SpellSelector()
{}

void SpellSelector::run()
{
	if (vc.A().pushed())
	{
		cursor = (cursor - 1 + num_handcards) % num_handcards;
		$P.sound_stack.push_back($P.snd_select);
	}
	else if (vc.S().pushed())
	{
		cursor = (cursor + 1) % num_handcards;
		$P.sound_stack.push_back($P.snd_select);
	}
	num_rest_cards = deck->NumRest();
	deck->current_card = cursor;
	this->num_rest_cards = deck->NumRest();
	age++;
	//deck->run();
}

void SpellSelector::renderNum(gfw::BaseMedia& media, int x, int y, int n)
{
	BaseGraphics* graphics = media.getGraphics();
	//Cd3d9->put(graph.getTexture(), x, y, 0, 1 + 12 * (n % 5), 392 + 22 * (n / 5), 12, 24, false, 0xffffffff);
	graphics->draw($P.img_spellboard, x, y, 0, 1 + 12 * (n % 5), 392 + 22 * (n / 5)
		, 12, 24, false, 0xffffffff);
}

void SpellSelector::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	//Cd3d9->put(graph.getTexture(), 0, 0, 0, 0, 0, 64, 384, false, 0xffffffff);
	graphics->draw($P.img_spellboard, 0, 0, 0, 0, 0, 64, 384, false, 0xffffffff);

	//Cd3d9->put(graph.getTexture(), 13, 75 + 54 * cursor, 0, 0, 440, 35, 35, false, 0xffffffff);

	int digit1 = num_rest_cards % 10;
	int digit10 = (num_rest_cards / 10) % 10;

	renderNum(media, 18, 34, digit10);
	renderNum(media, 29, 34, digit1);

	for (int i = 0; i < num_handcards; i++)
	{
		/*if (i >= deck->numCards())
		{
			graphics->draw($P.img_cards, 14, 76 + 54 * i, 0, 0, 32 * 4, 32, 32, false, 0xffffffff);
			continue;
		}*/
		Card card = deck->getCard(i);
		if (card.used || deck->DrawCounter()[i] > 0)
		{
			graphics->draw($P.img_cards, 14, 76 + 54 * i, 0, 0, 32 * 4, 32, 32, false, 0xffffffff);
			continue;
		}

		int refx = card.spell.thumnil_refx;
		int refy = card.spell.thumnil_refy;
		graphics->draw($P.img_cards, 14, 76 + 54 * i, 0, refx, refy, 32, 32, false, 0xffffffff);

		int digit1 = card.value % 10;
		int digit10 = (card.value / 10) % 10;

		//renderNum(50 + 13, 75 + 54*i, digit10);
		//renderNum(50 + 13 + 11, 75 + 54*i, digit1);
	}
	//int color = age / 20 % 2 ? 0xffffffff : 0xffff0000;
	int color = 0xffff0000;
	graphics->draw($P.img_spellboard, 13, 75 + 54 * cursor, 0, 0, 440, 35, 35, false, color);
	char text[] = "ddddddddddddddddd 99999";

}

bool SpellSelector::loadDeck()
{
	return false;
	/*SAVE s;
	if (!s.load())
		return false;

	deck = s.Data().deck;
	deck->reset();
	deck->shuffle();
	return true;*/
}