#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
using std::min;
using std::max;

RegisterMoverClass(PrologFujitsubo);
RegisterMoverClass(PrologHand);

void PrologFujitsubo::initialize()
{
	Effect::initialize(-1, -1, 0, 0, gmtr::Vector2D(0, 0), 60 * 9, gmtr::Vector2D(0, 0), 0, false);
	to_turn = false;
	writeName("effect_profuji", name);
	$P.sound_stack.push_back($P.snd_chime);
}
void PrologFujitsubo::run(jnr::JumpnRunData& data)
{
	turned = $P.turn_boss_dummy;
	//if (data.encount_boss >= 0 && ! to_turn && data.turn_boss_dummy)
	//{
	//	turned = !turned;
	//	to_turn = true;
	//}
	if (data.in_boss_battle)
	{
		alive = false;
	}
	Effect::run(data);
}
void PrologFujitsubo::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_refx = 0;

	if (age >= 60 * 4)
	{
		offset_refx = min(4, (age - 60 * 4) / 70 + 2);
	}
	else if (age / 45 % 2 == 1)
		offset_refx = 1;

	//graphics->draw($P.img_obj, 64 * 4, 64 * 3, 0
	//	, REFPOINT_SLEEPING_X, REFPOINT_SLEEPING_Y, 0, 0, false);
	graphics->draw($P.img_obj, 64 * 4, 64 * 3 + 10, 0
		, REFPOINT_SLEEPING_X + offset_refx * 64, REFPOINT_SLEEPING_Y, 64, 64, false);
}


void PrologHand::initialize()
{
	Effect::initialize(-1, -1, 0, 0, gmtr::Vector2D(0, 0), 60 * 5, gmtr::Vector2D(0, 0), 0, false);
	to_turn = false;
	writeName("effect_profuji", name);
}
void PrologHand::run(jnr::JumpnRunData& data)
{
	turned = $P.turn_boss_dummy;
	//if (data.encount_boss >= 0 && ! to_turn && data.turn_boss_dummy)
	//{
	//	turned = !turned;
	//	to_turn = true;
	//}
	if (data.in_boss_battle)
	{
		alive = false;
	}
	if (age == 25)
	{
		$P.sound_stack.push_back($P.snd_nyu);
	}
	if (age == 60 * 4 - 42)
	{
		data.camera.setQuake(25, 4);
		$P.sound_stack.push_back($P.snd_exp);
		generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y, 32, 32
			, gmtr::Vector2D(64 * 9 - 16, 64 * 4 + 32), 10, Vector2D(0, 0), 0, false, 0.25, 0.25, 8);
		generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32
			, gmtr::Vector2D(64 * 9 - 16, 64 * 4 + 32), 4, Vector2D(0, 0), 0);
	}
	if (age == 60 * 4 - 35)
	{
		$P.sound_stack.push_back($P.snd_exp);
		generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_X, REFPOINT_EXPLOSION_Y, 32, 32
			, gmtr::Vector2D(64 * 9, 64 * 4 + 32 + 16), 10, Vector2D(0, 0), 0, false, 0.25, 0.25, 8);
		generate<Effect>(*tl)->initialize(REFPOINT_EXPLOSION_WHITE_X, REFPOINT_EXPLOSION_WHITE_Y, 32, 32
			, gmtr::Vector2D(64 * 9, 64 * 4 + 32 + 16), 4, Vector2D(0, 0), 0);
	}

	Effect::run(data);
}
void PrologHand::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	//turned = $P.turn_boss_dummy;
	//Effect::render(media, data);
	int offset_refx = 0;

	int x = (age - 30) * 4;
	int dy = age / 10 % 2;

	//graphics->draw($P.img_obj, 64 * 4, 64 * 3, 0
	//	, REFPOINT_SLEEPING_X, REFPOINT_SLEEPING_Y, 0, 0, false);
	graphics->draw($P.img_obj, x, 64 * 1 + 24 + dy, 0
		, REFPOINT_BOY_X + 32 * 3, REFPOINT_BOY_Y, 32, 64, false);
	graphics->draw($P.img_obj, x, 64 * 1 + dy, 0
		, REFPOINT_HAND_X + 32 * 2, REFPOINT_HAND_Y, 32, 32, false);


}