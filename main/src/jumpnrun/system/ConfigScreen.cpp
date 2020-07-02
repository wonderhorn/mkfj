#include"jumpnrun/system/ConfigScreen.h"
#include"jumpnrun/system/Parmanent.h"
#include"utils/string_util.h"
using namespace gfw;
using namespace jnr;

ConfigScreen::ConfigScreen(gfw::Game* game): Screen(game)
{}

ConfigScreen::~ConfigScreen(){}

bool ConfigScreen::initialize()
{
	cursor = 0;
	age = 0;
	names =
	{
		"決定 /  攻撃",
		"キャンセル /  魔法",
		"ページ送り /  カード選択（上）",
		"ページ送り /  カード選択（下）",
		"ポーズ",
		//"ジャンプ",
		//"移動",
		"SE 音量",
		"BGM 音量",
		"もどる",
	};
	values =
	{
		"N/A",
		"N/A",
		"N/A",
		"N/A",
		"N/A",
		myutil::int2str($P.volume_se),
		myutil::int2str($P.volume_bgm),
		"",
	};
	return true;
}

void ConfigScreen::finalize()
{
	if($P.to_save)
		$P.save($P.save_file_name);
}

gfw::Screen::eScreenState ConfigScreen::run(gfw::BaseMedia& media)
{
	const auto &vc = *$P.players[0].vc;
	age++;
	counter_cursor++;
	timer_button.advance();

	if (vc.Down().pushed())
	{
		$P.sound_stack.push_back($P.snd_select);
		cursor++;
		if (cursor >= names.size())
			cursor = 0;
	}
	if (vc.Up().pushed())
	{
		$P.sound_stack.push_back($P.snd_select);

		cursor--;
		if (cursor < 0)
			cursor = names.size() - 1;

	}

	values[0] = std::string("ボタン") + myutil::int2str($P.joycont.btnidx_z);
	values[1] = std::string("ボタン") + myutil::int2str($P.joycont.btnidx_x);
	values[2] = std::string("ボタン") + myutil::int2str($P.joycont.btnidx_a);
	values[3] = std::string("ボタン") + myutil::int2str($P.joycont.btnidx_s);
	values[4] = std::string("ボタン") + myutil::int2str($P.joycont.btnidx_space);
	values[5] = myutil::int2str($P.volume_se);
	values[6] = myutil::int2str($P.volume_bgm);

	int pushed_button = $P.joycont.getPushedButtonId();
	if (pushed_button >= 0)
	{
		int dummy = 0;
		int* ptr = &dummy;
		switch(cursor)
		{
		default:
			break;
		case 0:
			ptr = &$P.joycont.btnidx_z;
			break;
		case 1:
			ptr = &$P.joycont.btnidx_x;
			break;
		case 2:
			ptr = &$P.joycont.btnidx_a;
			break;
		case 3:
			ptr = &$P.joycont.btnidx_s;
			break;
		case 4:
			ptr = &$P.joycont.btnidx_space;
			break;
		}
		*ptr = pushed_button;
	}

	if (cursor == 5)
	{
		if (vc.Z().pushed())
			$P.volume_se++;
		else if (vc.X().pushed())
			$P.volume_se--;
		if ($P.volume_se < 0)
			$P.volume_se = 0;
		if ($P.volume_se >= 10)
			$P.volume_se = 9;
	}
	$P.media.sounds.Volume($P.volume_se);

	if (cursor == 6)
	{
		if (vc.Z().pushed())
			$P.volume_bgm++;
		else if (vc.X().pushed())
			$P.volume_bgm--;
		if ($P.volume_bgm < 0)
			$P.volume_bgm = 0;
		if ($P.volume_bgm >= 10)
			$P.volume_bgm = 9;
	}
	$P.media.musics.setVolume($P.volume_bgm);

	if (cursor == names.size() - 1)
	{
		if (vc.Z().pushed())
			return Screen::eScreenState::Finished;
	}
	//if (vc.Space().pushed())
	//	return Screen::eScreenState::Finished;
	return Screen::Continuing;
}
void ConfigScreen::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->clear(0, 0, 255, 255);
	graphics->draw($P.img_paper, 0, 0, 0, 0, 0, 640, 480, false, 0xffffffff);

	for (int i = 0; i < values.size(); i++)
	{
		graphics->drawString(names[i], 120, 60 + i * 45, 400, 20, 0, 0, 0, 1);
		graphics->drawString(values[i], 360, 60 + i * 45, 400, 20, 0, 0, 0, 1);
	}
	graphics->draw($P.img_cursor, 65, 60 + cursor * 45 - 5, 0, 0, 0, 40, 80, false);
}