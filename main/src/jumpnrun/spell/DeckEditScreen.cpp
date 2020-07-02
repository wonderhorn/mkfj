#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/spell/DeckEditScreen.h"
#include"jumpnrun/spell/SpellName2Text.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/ai/AI_DeckEdit.h"
#include"utils/string_util.h"
#include<string>
#include<algorithm>
using namespace gfw;
using namespace jnr;
using namespace spl;
static const int NUM_ROWS = 7;

using std::min;
using std::max;

static int &getCardNum(DeckTab &tub, int card_id)
{
	static int dummy = 0;
	for (auto& t : tub.cards)
	{
		if (std::get<0>(t) == card_id)
			return std::get<1>(t);
	}
	tub.cards.push_back(std::tuple<int, int>(card_id, 0));
	return std::get<1>(tub.cards[tub.cards.size() - 1]);
}

DeckTab::DeckTab(): name("noname"){}

int DeckTab::numCards()const
{
	int rtn = 0;
	for (auto t : cards)
		rtn += std::get<1>(t);
	return rtn;
}

Deck DeckTab::writeDeck()const
{
	Deck rtn;
	int num_cards = 0;
	for (auto tup : cards)
	{
		num_cards += std::get<1>(tup);
	}
	rtn.initialize(num_cards);

	int counter = 0;
	for (auto tup : cards)
	{
		for (int i = 0; i < std::get<1>(tup); i++)
		{
			Card& c = rtn.getCard(counter);
			c.value = std::get<0>(tup);
			if(c.value < 0)
			continue;
			c.used = false;
			c.spell = spl::Spell::spells[c.value];
			counter++;
		}
	}
	return rtn;
}

DeckTab DeckTab::fromDeck(const Deck& deck)
{
	DeckTab rtn;
	for (int i = 0; i < deck.numCards(); i++)
	{
		int c = deck.getCardConst(i).value;
		auto it = std::find_if(rtn.cards.begin(), rtn.cards.end()
			, [c](const std::tuple<int, int> &t){return c == std::get<0>(t); });
		if (it == rtn.cards.end())
		{
			rtn.cards.push_back(std::tuple<int, int>(c, 1));
		}
		else
			std::get<1>(*it)++;
	}
	return rtn;
}


DeckTab DeckTab::gachaInitial()
{
	DeckTab rtn;
	rtn.name = "gapha";
	for (int i = 0; i < Spell::spells.size(); i++)
	{
		if ($P.mylibrary.count(i))
		{
			rtn.cards.push_back(std::tuple<int, int>(i, 0));
		}
	}
	return rtn;
}

DeckTab DeckTab::allCards()
{
	DeckTab rtn;
	rtn.name = "all cards";
	for (int i = 0; i < Spell::NUM_SPELL; i++)
	{
		rtn.cards.push_back(std::tuple<int, int>(i, 10));
	}

	// sort by card 
	std::sort(
		rtn.cards.begin(),
		rtn.cards.end(),
		[](const auto& x, const auto& y) 
		{
			return Spell::spells[std::get<0>(x)].id < Spell::spells[std::get<0>(y)].id;
		}
	);

	return rtn;
}

DeckTab DeckTab::cardsInBag()
{
	DeckTab rtn;
	rtn.name = "cards in my bag";
	for (int i = 0; i < Spell::spells.size(); i++)
	{
		if ($P.mylibrary.count(i))
		{
			rtn.cards.push_back(std::tuple<int, int>(i, $P.mylibrary[i]));
		}
	}

	// sort by card 
	std::sort(
		rtn.cards.begin(),
		rtn.cards.end(),
		[](const auto& x, const auto& y)
	{
		return Spell::spells[std::get<0>(x)].id < Spell::spells[std::get<0>(y)].id;
	}
	);

	return rtn;
}


bool DeckEditScreen::initialize()
{
	prev_camera_x = $P.camera.X();
	prev_camera_y = $P.camera.Y();
	cursor_tab = 0;
	cursor = 0;
	scroll = 0;
	age = 0;
	counter_cursor = 0;
	tabs.push_back(DeckTab::fromDeck($P.players[0].deck));
	//tabs.push_back(DeckTab::allCards());
	//all_cards = DeckTab::allCards();
	all_cards = DeckTab::cardsInBag();

	bool result;
	result = tl.initialize();
	gfw::Object::tl = &tl;

	auto* ai_tmp = new ai::AI_DeckEdit();
	ai_tmp->initialize();
	ai = ai_tmp;

	AIPlayer* p = generate<AIPlayer>(tl);
	static_cast<ai::AI_DeckEdit*>(ai)->initialize();
	p->initialize(0, 0, gmtr::Vector2D(64, 64 * 3 ), 0, false, ai);
	p->use_spellset = false;
	p->MP() = 999999;
	p->MPMax() = 999999;
	p->HP() = 999999;
	p->HPMax() = 999999;
	p->UsingCardFromDeck(false);
	p->GoOutOfCamera(true);
	player = Object::Pipe(p, tl);
	jnr::StageDataLoader().createPlainStage(3, 4, &tl, stg, om);
	data.pullFromParmanent();
	data.camera.outOfStage = true;
	return true;
}
void DeckEditScreen::finalize()
{
	$P.players[0].deck = tabs[cursor_tab].writeDeck();
	if($P.to_save)
		$P.save($P.save_file_name);
	//$P.players[0].deck.shuffle();
	$P.camera.setPos(prev_camera_x, prev_camera_y);
	if (ai)
	{
		delete ai;
		ai = nullptr;
	}
}

Screen::eScreenState DeckEditScreen::run(gfw::BaseMedia& media)
{
	const auto &vc = *$P.players[0].vc;
	age++;
	counter_cursor++;
	timer_button.advance();

	stg.run(data);
	Mover::stg = &stg;
	Mover::tl = &tl;
	int num_enemy = 0;
	for (auto t = tl.begin(); t != tl.end();)
	{
		Object* obj = getObject(t);
		((Mover*)obj)->run(data);
		num_enemy++;

		if (obj->alive)
			t++;
		else
		{
			delete (Object*)t->workspace;
			t = tl.remove(t);
		}
	}
	om.run();
	if(num_enemy >= 30)
	{
		for (auto t = tl.begin(); t != tl.end();)
		{
			Object* obj = getObject(t);
			Mover* mvr = ((Mover*)obj);
			if (mvr->Name() != "character_aiplayer") 
			{
				delete (Object*)t->workspace;
				t = tl.remove(t);
			}
			else
				t++;
		}
	}

	if (Player* p = (Player*)player.Dst(tl))
	{
		auto myp = p->P();
		data.camera.setPos(-32, -20 + 64);
		p->setXSpell(std::get<0>(all_cards.cards[scroll + cursor]));
	}
	if ($P.sound_on)
	{
		while (!$P.sound_stack.empty())
		{
			media.getSounds()->play($P.sound_stack.front());
			$P.sound_stack.pop_front();
		}
	}

	if (vc.Z().pushed())
	{
		int idx = cursor + scroll;
		if (all_cards.cards.size() > idx)
		{
			//auto tup = tabs[cursor_tab].cards[card_index];
			int card_id = std::get<0>(all_cards.cards[idx]);
			//int& num = std::get<1>(all_cards.cards[idx]);
			int& num = getCardNum(tabs[cursor_tab], card_id);

			int max_num = 0;
			for (auto t : all_cards.cards)
			{
				if (std::get<0>(t) == card_id)
					max_num = std::get<1>(t);
			}

			if (num < max_num && num < 3)
			{
				num++;
				$P.sound_stack.push_back($P.snd_select);
			}
			else 
				$P.sound_stack.push_back($P.snd_cancel);
		}

	}
	else if (vc.X().pushed())
	{
		int idx = cursor + scroll;
		if (all_cards.cards.size() > idx)
		{
			//auto tup = tabs[cursor_tab].cards[card_index];
			int card_id = std::get<0>(all_cards.cards[idx]);
			//int& num = std::get<1>(all_cards.cards[idx]);
			int& num = getCardNum(tabs[cursor_tab], card_id);

			//int& num = std::get<1>(tabs[cursor_tab].cards[cursor + scroll]);
			//int id = std::get<0>(tabs[cursor_tab].cards[idx]);
			int max_num = 0;
			for (auto t : all_cards.cards)
			{
				if (std::get<0>(t) == card_id)
					max_num = std::get<1>(t);
			}

			if (num > 0)
				num--;
			$P.sound_stack.push_back($P.snd_cancel);
		}
	}

	if (vc.Right().pushed())
	{
		cursor_tab++;
		cursor_tab %= tabs.size();
	}
	else if (vc.Left().pushed())
	{
		cursor_tab += tabs.size() - 1;
		cursor_tab %= tabs.size();
	}

	if (vc.Down().pushed() || vc.Up().pushed() || vc.A().pushed() || vc.S().pushed())
	{
		bool result;
		result = tl.initialize();
		gfw::Object::tl = &tl;

		/*auto* ai_tmp = new ai::AI_DeckEdit();
		ai_tmp->initialize();
		ai = ai_tmp;*/

		static_cast<ai::AI_DeckEdit*>(ai)->initialize();
		AIPlayer* p = generate<AIPlayer>(tl);
		p->initialize(0, 0, gmtr::Vector2D(64, 64 * 3 - 1), 0, false, ai);
		p->use_spellset = false;
		p->MP() = 999999;
		p->MPMax() = 999999;
		p->HP() = 999999;
		p->HPMax() = 999999;
		p->UsingCardFromDeck(false);
		p->GoOutOfCamera(true);
		player = Object::Pipe(p, tl);
	}

	int current_num_rows = min(NUM_ROWS, (int)all_cards.cards.size() - scroll);
	if (vc.Down().pushed() || timer_button.alarm() && vc.Down().isDown())
	{
		counter_cursor = 0;
		if (vc.Down().pushed())
			timer_button.reset(15);
		else
			timer_button.reset(5);
		cursor++;
		if (current_num_rows <= cursor)
		{
			cursor = current_num_rows - 1;
			scroll++;
		}
	}
	else if (vc.Up().pushed() || timer_button.alarm() && vc.Up().isDown())
	{
		counter_cursor = 0;
		if (vc.Up().pushed())
			timer_button.reset(15);
		else
			timer_button.reset(5);
		cursor--;
		if (0 > cursor)
		{
			cursor = 0;
			scroll--;
		}
	}

	if (vc.S().pushed() || timer_button.alarm() && vc.S().isDown())
	{
		counter_cursor = 0;
		if (vc.S().pushed())
			timer_button.reset(15);
		else
			timer_button.reset(5);
		scroll += NUM_ROWS;
	}
	else if (vc.A().pushed() || timer_button.alarm() && vc.A().isDown())
	{
		counter_cursor = 0;
		if (vc.A().pushed())
			timer_button.reset(15);
		else
			timer_button.reset(5);
		scroll -= NUM_ROWS;
	}

	//int MAX_SCROLL = tabs[cursor_tab].cards.size() - NUM_ROWS ;
	int MAX_SCROLL = all_cards.cards.size() - NUM_ROWS;
	if (MAX_SCROLL < 0)
		MAX_SCROLL = 0;
	if (scroll < 0)
	{
		timer_button.reset(20);
		scroll = MAX_SCROLL;
		if (MAX_SCROLL == 0)
			scroll = 0;
		cursor = current_num_rows - 1;
	}
	if (scroll > MAX_SCROLL)
	{
		timer_button.reset(20);
		scroll -= MAX_SCROLL + 1;
		cursor = 0;
	}

	if (vc.Space().pushed())
		return Screen::eScreenState::Finished;
	return Screen::eScreenState::Continuing;
}

static void renderNumber(gfw::BaseMedia& media, int n, int x, int y)
{
	BaseGraphics* graphics = media.getGraphics();
	int one = n % 10;
	int ten = (n % 100) / 10;

	//151, 72
	graphics->draw($P.img_deckedit, x + 13, y, 0, 248 + 13 * one, 72, 13, 33, false, 0xffffffff);
	graphics->draw($P.img_deckedit, x, y, 0, 248 + 13 * ten
		, 72, 13, 33, false, 0xffffffff);
}

void DeckEditScreen::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	static const int init_x = 192, init_y = 60;
	static const int platew = 352, plateh = 48;

	graphics->draw($P.img_paper, 0, 0, 0, 0, 0, 640, 480);
	graphics->draw($P.img_deckedit, 52 - 25, 73 - 5, 0, 15, 15, 202, 149, false, 0xffffffff);
	//graphics->drawString(tabs[cursor_tab].name, 180, 20, 200, 20);
	graphics->drawString("デッキ編集画面", 60, 20, 200, 20);

	for (int i = 0; i < NUM_ROWS; i++)
	{
		int card_index = scroll + i;
		//if (tabs[cursor_tab].cards.size() <= card_index)
		//	break;
		
		auto& tab = all_cards;
		if (tab.cards.size() <= card_index)
			break;
		int card_id = std::get<0>(tab.cards[card_index]);
		int card_num = std::get<1>(tab.cards[card_index]);
		auto s = spl::Spell::spells[card_id];
		graphics->draw($P.img_deckedit, init_x + plateh + 4, init_y + i * (plateh + 4), 0, 
			247, 128, platew, plateh);
		graphics->draw($P.img_cards, init_x + plateh + 4 + 16, init_y + i * (plateh + 4) + 8, 0
			, s.thumnil_refx, s.thumnil_refy, 32, 32);
		renderNumber(media, getCardNum(tabs[cursor_tab], card_id), 
			548 + 16 - 48 + 6, init_y + 8 + i * (plateh + 4));
		//renderNumber(media, 10, 548 + 16 - 16 + 12, init_y + 8 + i * (plateh + 4));
		graphics->draw($P.img_deckedit, 548 + 16 - 16 + 6, init_y + 8 + i * (plateh + 4)
			, 0, 248 + 13 * 10 + 1, 72, 12, 33, false, 0xffffffff);

		int nl = $P.mylibrary.count(card_id) ? $P.mylibrary[card_id] : 0;
		renderNumber(media, nl, 548 + 16, init_y + 8 + i * (plateh + 4));

		std::string spellname = SpellName2Text::Name(s.name);
		graphics->drawString(spellname, init_x + plateh + 4 + 76, init_y + i * (plateh + 4) + 14
			, 240, 20, 255, 255, 255);
	}
	if (counter_cursor % 40 <= 30)
		graphics->draw($P.img_deckedit, init_x + plateh + 4 - 1, init_y + cursor * (plateh + 4) - 1
		, 0, 247, 15, 354, 50);
	else
		graphics->draw($P.img_deckedit, init_x + plateh + 4 - 1, init_y + cursor * (plateh + 4) - 1
		, 0, 248, 190, 354, 50, false);

	//auto tup = tabs[cursor_tab].cards[scroll + cursor];
	auto tup = all_cards.cards[scroll + cursor];
	int card_id = std::get<0>(tup);
	int card_num = std::get<1>(tup);
	auto s = spl::Spell::spells[card_id];

	std::string rule_text = SpellName2Text::Text(s.name);
	std::vector<std::string> lines = myutil::split(rule_text, "\n");

	std::string rarity = "C";
	switch (s.rarity)
	{
	default:	rarity = "C"; break;
	case 'u':	rarity = "U"; break;
	case 'r':	rarity = "R"; break;
	case 's':	rarity = "S"; break;
	}

	graphics->drawString(std::string("コスト: ") + myutil::int2str(s.manacost) +
		"　　パワー: " + (s.power == 0 ? "--": myutil::int2str(s.power))
		, 32 - 2 + 12, 260 - 2 + 10
		, 240, 20, 255, 255, 255);
	graphics->drawString("レアリティ: " + rarity
		, 32 - 2 + 12, 260 - 2 + 10 + 20
		, 240, 20, 255, 255, 255);
	for (int i = 0; i < lines.size(); i++)
	{
		graphics->draw($P.img_deckedit, 32 - 2, 280 + i * 20 - 2 + 35
			, 0, 16, 172, 200, 20);
		graphics->drawString(lines[i], 32, 280 + i * 20 + 35
			, 240, 20, 255, 255, 255);
	}

	int MAX_SCROLL = all_cards.cards.size() - NUM_ROWS;
	double scroll_rate = (double)(scroll + cursor) / (MAX_SCROLL + NUM_ROWS - 1);
	graphics->drawLine(624, 48, 624, 436, 192, 192, 0, 255, 2);
	graphics->draw($P.img_deckedit, 624 - 2, 48 + (436 - 48) * scroll_rate
		, 0, 252, 111, 7, 10);

	stg.render(media, data);
	for (auto t = tl.begin(); t != tl.end(); t++)
	{
		Object* obj = getObject(t);
		((Mover*)obj)->render(media, data);
	}

	graphics->drawString("操作", 40, 380, 200, 20);
	graphics->drawString("Z: カードをデッキに足す", 40, 400, 200, 20);
	graphics->drawString("X: カードをデッキから減らす", 40, 420, 200, 20);
	graphics->drawString("↑↓: カーソル移動", 40, 440, 200, 20);
	graphics->drawString("SPACE: 編集終了", 40, 460, 200, 20);


	//card_gallery.render(media);
}