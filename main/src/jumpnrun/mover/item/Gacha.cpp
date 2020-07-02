#define _USE_MATH_DEFINES
#include<cmath>
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/Item.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/spell/Spell.h"
#include"jumpnrun/spell/SpellName2Text.h"
#include"utils/string_util.h"
using namespace jnr;
using namespace gfw;
using namespace gmtr;
using std::max;
using std::min;
#define $V gmtr::Vector2D

RegisterMoverClass(Gacha);

static void cpyString(char buf[JNR_FLAG_SIZE], const std::string& str)
{
	assert(str.size() < JNR_FLAG_SIZE);
	for (int i = 0; i < str.size(); i++)
	{
		buf[i] = str[i];
	}
	buf[JNR_FLAG_SIZE - 1] = '\0';
}


void Gacha::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, double mass)
{
	Item::initialize(refx, refy, p, owner, power, $V(0, 0), mass);
	touch_and_dissapear = false;
	lifetime = INT_MAX;
	this->rotate_to_direction = false;
	writeName("item_gacha", name);
}

void Gacha::interact(jnr::Character& chara, jnr::JumpnRunData& data)
{
	$V center = phys.p + $V(width / 2, height / 2);
	if (chara.Shape().includes(center))
	{
		if (chara.Name() == "character_player")
			timer_show_down_button = 12;
		chara.onItem();
		if (chara.pickingItem())
		{
			$P.sound_stack.push_back($P.snd_select);
			data.open_gacha = true;
		}

		if ($P.card_from_gacha >= 0)
		{
			generate<GetCard>(*tl)->initialize(
				REFPOINT_CARD_X, REFPOINT_CARD_Y, 
				this->P() + $V(0, -96), 
				0, 1.0,
				$P.card_from_gacha);
			gfw::generate<Particles>(*chara.tl)->initialize(
				REFPOINT_STAR1_X, REFPOINT_STAR1_Y, 32, 32,
				this->P() + $V(0, -96) + $V(0, 0), 8, $V(0, 0), 0);
			$P.card_from_gacha = -1;
		}
	}
}

void Gacha::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	int offset_y = 0;
	graphics->draw($P.img_obj, phys.p.x - data.camera.X(), phys.p.y - offset_y - data.camera.Y(), 0
		, refx, refy, width, height, turned);

	if (timer_show_down_button > 0)
	{
		if ((age % 60) < 40)
			graphics->drawString("Бе",
				phys.p.x + 8 - data.camera.X(), phys.p.y + height - data.camera.Y(),
				24, 24, 0, 1, 0, 1);
	}
}