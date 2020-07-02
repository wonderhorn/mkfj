#include"jumpnrun/system/JumpnRunData.h"
#include"jumpnrun/system/Parmanent.h"
#include<iostream>
using namespace jnr;
using std::max;
using std::min;
void JumpnRunData::pullDeckFromParmanent()
{
	for (int i = 0; i < $P.players.size(); i++)
	{
		players[i].deck = $P.players[i].deck;
		//players[i].deck_tmp = $P.players[i].deck_tmp;
	}
}

void JumpnRunData::pullFromParmanent()
{
	//std::cout << "Pulling data from parmanent" << std::endl;
	this->camera = $P.camera;
	this->change_stage = $P.change_stage;
	this->encount_boss = $P.encount_boss;
	this->message = $P.message;
	this->message_time = $P.message_time;
	this->next_stage_name = $P.next_stage_name;
	this->next_start_p = $P.next_start_p;
	this->turned = $P.turned;
	//this->players = $P.players;
	this->players.clear();
	for (int i = 0; i < $P.players.size(); i++)
	{
		PlayerData& d = $P.players[i];
		PlayerData newd();
		players.push_back($P.players[i]);
		//players[i].deck = $P.players[i].deck;
		//players[i].deck_tmp = $P.players[i].deck_tmp;
	}

	this->cleared_stage = $P.cleared_stage;
	//this->num_boss1_beaten = $P.num_boss1_beaten;
	//this->num_boss2_beaten = $P.num_boss2_beaten;
	//this->num_boss3_beaten = $P.num_boss3_beaten;
	this->num_enemies_beaten = $P.num_enemies_beaten;
	this->num_player_beaten = $P.num_player_beaten;
}
void JumpnRunData::pushToParmanent()const
{
	//std::cout << "Pushing data to parmanent" << std::endl;
	$P.camera = this->camera;
	$P.change_stage = this->change_stage;
	$P.encount_boss = this->encount_boss;
	$P.message = this->message;
	$P.message_time = this->message_time;
	$P.next_stage_name = this->next_stage_name;
	$P.next_start_p = this->next_start_p;
	$P.turned = this->turned;
	//$P.players = this->players;
	/*
	$P.players.clear();
	for (auto p : this->players)
	{
		$P.players.push_back(p);
	}*/

	for (int i = 0; i < this->players.size(); i++)
	{
		auto& p = $P.players[i];
		auto& p_this = this->players[i];
		p.hp_current = p_this.hp_current;
		p.hp_max = p_this.hp_max;
		p.isActive = p_this.isActive;
		p.max_score = p_this.max_score;
		p.mp_current = p_this.mp_current;
		p.mp_max = p_this.mp_max;
		p.score = p_this.score;
		p.deck = p_this.deck;
		p.deck_tmp = p_this.deck_tmp;
		p.max_score = max(p.max_score, p_this.score);
	}

	$P.cleared_stage = cleared_stage;
	//$P.num_boss1_beaten = num_boss1_beaten;
	//$P.num_boss2_beaten = num_boss2_beaten;
	//$P.num_boss3_beaten = num_boss3_beaten;
	$P.num_enemies_beaten = num_enemies_beaten;
	$P.num_player_beaten = num_player_beaten;
}