#include"jumpnrun/stage/ItemDropper.h"
#include"jumpnrun/mover/Item.h"
#include"jumpnrun/GRAPHICS.h"
using namespace jnr;

void ItemDropper::drop(gfw::Tasklist& tl
	, const gmtr::Vector2D& p, const std::string& dropper_name)
{
	double u = (double)$P.rand.get() / RAND_MAX;
	if (dropper_name.substr(0, 15) == "character_enemy")
	{
		if (dropper_name.substr(16, 4) == "mole")
		{

		}
		else if (dropper_name.substr(16, 3) == "elf")
		{
			if (u <= 0.6)
			{
				auto* ptr = gfw::generate<GetCard>(tl);
				ptr->initialize(REFPOINT_CARD_X, REFPOINT_CARD_Y, p - gmtr::Vector2D(16, 16)
					, -1, 1, 39);
			}
		}
		else if (dropper_name.substr(16, 4) == "frog")
		{
			/*if (u <= 0.2)
			{
				auto* ptr = gfw::generate<GetCard>(tl);
				ptr->initialize(REFPOINT_CARD_X, REFPOINT_CARD_Y, p - gmtr::Vector2D(16, 16)
					, -1, 1, 7);
			}*/
		}
		else if (dropper_name.substr(16, 3) == "car")
		{
			if (u <= 0.5)
			{
				auto* ptr = gfw::generate<GetCard>(tl);
				ptr->initialize(REFPOINT_CARD_X, REFPOINT_CARD_Y, p - gmtr::Vector2D(16, 16)
					, -1, 1, 47);
			}
		}
		else if (dropper_name.substr(16, 5) == "snail")
		{
			if (u <= 0.4)
			{
				auto* ptr = gfw::generate<GetCard>(tl);
				ptr->initialize(REFPOINT_CARD_X, REFPOINT_CARD_Y, p - gmtr::Vector2D(16, 16)
					, -1, 1, 25);
			}
		}
		else if (dropper_name.substr(16, 6) == "candle")
		{
			if (u <= 0.4)
			{
				auto* ptr = gfw::generate<GetCard>(tl);
				ptr->initialize(REFPOINT_CARD_X, REFPOINT_CARD_Y, p - gmtr::Vector2D(16, 16)
					, -1, 1, 9);
			}
		}
		else if (dropper_name.substr(16, 6) == "cannon")
		{
			if (u <= 0.25)
			{
				auto* ptr = gfw::generate<GetCard>(tl);
				ptr->initialize(REFPOINT_CARD_X, REFPOINT_CARD_Y, p - gmtr::Vector2D(16, 16)
					, -1, 1, 55);
			}
		}
		else if (dropper_name.substr(16, 9) == "jellyfish")
		{
			if (u <= 0.4)
			{
				auto* ptr = gfw::generate<GetCard>(tl);
				ptr->initialize(REFPOINT_CARD_X, REFPOINT_CARD_Y, p - gmtr::Vector2D(16, 16)
					, -1, 1, 19);
			}
		}
		else if (dropper_name.substr(16, 10) == "mandragora")
		{
			if (u <= 0.3)
			{
				auto* ptr = gfw::generate<GetCard>(tl);
				ptr->initialize(REFPOINT_CARD_X, REFPOINT_CARD_Y, p - gmtr::Vector2D(16, 16)
					, -1, 1, 56);
			}
		}
		//else if (dropper_name.substr(16, 4) == "hand" && $P.encount_boss == 7)
		else if (dropper_name.substr(16, 4) == "hand")
		{
			if (u <= 0.3)
			{
				auto* ptr = gfw::generate<Item>(tl);
				ptr->initialize(REFPOINT_HEART_X, REFPOINT_HEART_Y, p - gmtr::Vector2D(16, 16)
					, -1, 20, gmtr::Vector2D(0, -4), 1);
				ptr->hp_gain = 10;
				ptr->mp_gain = 0;
				ptr->score_gain = 10;
			}
		}
		else if (u <= 0.25)
		{
			auto* ptr = gfw::generate<Item>(tl);
			ptr->initialize(REFPOINT_JEWEL_X, REFPOINT_JEWEL_Y + 32, p - gmtr::Vector2D(16, 16)
				, -1, 20, gmtr::Vector2D(0, -4), 1);
			ptr->hp_gain = 0;
			ptr->mp_gain = 10;
			ptr->score_gain = 10;
		}
	}
}