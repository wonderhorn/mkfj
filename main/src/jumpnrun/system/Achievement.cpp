#include"jumpnrun/system/Achievement.h"
#include"jumpnrun/system/Parmanent.h"

using namespace jnr;

void Achievement::calc()
{
	//int num_cards, num_card_species, stage_clear, enemies_beaten, bosses_beaten, you_beaten;
	num_cards = 0;
	num_card_species = 0;
	fujitsubo_power = 0;
	num_max_card_species = 0;

	for (auto t : $P.mylibrary)
	{
		int n = t.second;
		num_cards += n;
		num_card_species += 1;
	}

	stage_clear = $P.cleared_stage;
	enemies_beaten = $P.num_enemies_beaten;
	max_score = $P.players[0].max_score;
	scenario_cleared = $P.getFlag("endwatched");
	total_damage = $P.total_damage;


	fujitsubo_power = ($P.players[0].hp_max - 50) / 10
		+ ($P.players[0].mp_max - 50) / 10;

}