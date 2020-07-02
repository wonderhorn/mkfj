#include"jumpnrun/system/Achievement.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/GRAPHICS.h"
#include"utils/string_util.h"
#define _USE_MATH_DEFINES
#include<math.h>
using namespace gfw;
using namespace jnr;

AchievementScreen::AchievementScreen(gfw::Game* game) : Screen(game)
{}

AchievementScreen::~AchievementScreen(){}

bool AchievementScreen::initialize()
{
	age = 0;
	return true;
}

void AchievementScreen::finalize(){}

gfw::Screen::eScreenState AchievementScreen::run(gfw::BaseMedia& media)
{
	age++;

	const auto &vc = *$P.players[0].vc;
	ac.calc();

	names =
	{
		"持っているカードの数: ",
		"持っているカードの種類: ",
		"倒した敵の数: ",
		"ふじつぼの強さ: ",
		"受けたダメージ: ",
		"最高スコア",
		"シナリオをクリア",
	};

	values =
	{
		myutil::int2str(ac.num_cards) + " / 70",
		myutil::int2str(ac.num_card_species) + " / 50",
		myutil::int2str(ac.enemies_beaten) + " / 100",
		myutil::int2str(ac.fujitsubo_power) + " / 10",
		myutil::int2str(ac.total_damage) + " / 300",
		myutil::int2str(ac.max_score) + " / 1000",
		"",
	};

	stars =
	{
		ac.num_cards >= 70,
		ac.num_card_species >= 50,
		ac.enemies_beaten >= 100,
		ac.fujitsubo_power >= 10,
		ac.total_damage >= 300,
		ac.max_score >= 1000,
		ac.scenario_cleared,
	};

	int num_stars = 0;
	for (int star : stars)
		num_stars += star;
	if(num_stars >= 1)
		$P.flags["star1"] =1;
	if (num_stars >= 3)
		$P.flags["star3"] = 1;
	if (num_stars >= 5)
		$P.flags["star5"] = 1;

	if (vc.Space().pushed())
		return Screen::eScreenState::Finished;
	return Screen::Continuing;
}
void AchievementScreen::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->clear(0, 0, 255, 255);
	graphics->draw($P.img_paper, 0, 0, 0, 0, 0, 640, 480, false, 0xffffffff);

	for (int i = 0; i < values.size(); i++)
	{
		graphics->drawString(names[i], 120, 120 + i * 45, 400, 20, 0, 0, 0, 1);
		graphics->drawString(values[i], 360, 120 + i * 45, 400, 20, 0, 0, 0, 1);
		if (stars[i] >= 1)
		{
			//graphics->draw($P.img_obj, 440, 120 + i * 45 - 4, 0, REFPOINT_STAR1_X, REFPOINT_STAR1_Y
			//	, 32, 32, false, 0xffffffff);

			graphics->drawSpinResize($P.img_obj, 440, 120 + i * 45 - 4, 0, REFPOINT_STAR1_X, REFPOINT_STAR1_Y
				, 32, 32, false, 0xffffffff, 1, 1, 2 * M_PI * (age % 120) / 120);
		}
	}
	/*graphics->draw($P.img_cursor, 65, 60 + cursor * 60 - 5, 0, 0, 0, 40, 80, false);*/
	//graphics->drawString(names[i], 120, 60 + i * 60, 400, 20, 0, 0, 0, 1);
}