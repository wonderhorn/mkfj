#include"jumpnrun/spell/CardGalleryScreen.h"
#include"jumpnrun/spell/SpellName2Text.h"
#include"jumpnrun/system/Parmanent.h"
#include"utils/string_util.h"

using namespace gfw;
using namespace jnr;
using namespace spl;

const int NUM_COLUMN = 10;
const int NUM_ROW = 5;

CardGalleryScreen::CardGalleryScreen(gfw::Game* game) : gfw::Screen(game)
{}

bool CardGalleryScreen::initialize() 
{
	this->cursor_x = 0;
	this->cursor_y = 0;
	timer_button.reset(0);
	return true;
}

void CardGalleryScreen::finalize() {}

Screen::eScreenState CardGalleryScreen::run(gfw::BaseMedia& media)
{
	const auto &vc = *$P.players[0].vc;
	age++;
	counter_cursor++;
	timer_button.advance();

	const int cursor_freeze_time = 12;
	if (timer_button.alarm()) 
	{
		if (vc.Right().isDown())
		{
			cursor_x++;
			timer_button.reset(cursor_freeze_time);
		}
		if (vc.Left().isDown())
		{
			cursor_x--;
			timer_button.reset(cursor_freeze_time);
		}
		if (vc.Down().isDown())
		{
			cursor_y++;
			timer_button.reset(cursor_freeze_time);
		}
		if (vc.Up().isDown())
		{
			cursor_y--;
			timer_button.reset(cursor_freeze_time);
		}
	}
	cursor_x += NUM_COLUMN;
	cursor_x %= NUM_COLUMN;
	cursor_y += NUM_ROW;
	cursor_y %= NUM_ROW;

	if (vc.Space().pushed())
		return Screen::eScreenState::Finished;
	return Screen::eScreenState::Continuing;
}


void CardGalleryScreen::render(gfw::BaseMedia& media) 
{
	BaseGraphics* graphics = media.getGraphics();
	const int board_x = 30;
	const int board_y = 48;
	const int board_w = 589;
	const int board_h = 565 - 246;

	graphics->draw($P.img_paper, 0, 0, 0, 0, 0, 640, 480);

	//board
	graphics->draw($P.img_deckedit, board_x, board_y, 0, 
		0, 246, board_w, board_h, false, 0xffffffff);

	for (int iy = 0; iy < NUM_ROW; iy++) 
	{
		for (int ix = 0; ix < NUM_COLUMN; ix++) 
		{
			const int x = 25 + 56 * ix;
			const int y = 20 + 61 * iy;
			graphics->draw($P.img_cards, board_x + x, board_y + y, 0,
				0, 32 * 4, 32, 32, false, 0xffffffff);
		}
	}

	const int cursor_idx = cursor_x + cursor_y * NUM_COLUMN + 1;
	Spell* spell_under_cursor = nullptr;
	for(auto int_tup: $P.mylibrary)
	{
		int cid = int_tup.first;
		const auto& card = Spell::spells[cid];
		const int position_idx = card.id - 1;
		const int ix = position_idx % NUM_COLUMN;
		const int iy = position_idx / NUM_COLUMN;
		const int x = 25 + 56 * ix;
		const int y = 20 + 61 * iy;
		graphics->draw($P.img_cards, board_x + x, board_y + y, 0,
			card.thumnil_refx, card.thumnil_refy, 32, 32, false, 0xffffffff);

		// keep info of the card under the cursor
		if (card.id == cursor_idx) 
		{
			spell_under_cursor = &Spell::spells[cid];
		}
	}

	// cursor
	const int cx = 25 + 56 * cursor_x - 2;
	const int cy = 20 + 61 * cursor_y - 2;
	graphics->draw($P.img_deckedit, board_x + cx, board_y + cy, 0,
		0, 576, 36, 36, false, 0xffffffff);

	if (spell_under_cursor) 
	{
		auto s = *spell_under_cursor;

		std::string card_name = SpellName2Text::Name(s.name);
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

		graphics->drawString(card_name
			, 32 - 2 + 12, 380
			, 240, 20, 255, 255, 255);
		graphics->drawString(std::string("コスト: ") + myutil::int2str(s.manacost) +
			"　　レアリティ: " + rarity
			, 32 - 2 + 12 + 32, 380 + 24
			, 240, 20, 255, 255, 255);
		for (int i = 0; i < lines.size(); i++)
		{
			graphics->draw($P.img_deckedit, 332 - 2, 380 + 12 + i * 20 - 2
				, 0, 16, 172, 200, 20);
			graphics->drawString(lines[i], 332, 380 + 12 + i * 20
				, 240, 20, 255, 255, 255);
		}

	}

}
