#include"jumpnrun/mover/character/Character.h"

namespace jnr
{
	namespace enm{ class Enemy; };
	class EnemyGroup : public Character
	{
	public:
		EnemyGroup(){}
		enum EnemyType{ Bee, Soldier, Hand, Hand2};
		virtual void initialize(gmtr::Vector2D p, OWNERHANDLE owner, EnemyType type, const std::string& flag_id = "");
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data){}
		virtual void die(jnr::JumpnRunData& data);
		virtual const gfw::VirtualController& Controller(){ return mc; }
		enm::Enemy* genEnemy();

	protected:
		gfw::ManualController mc;
		int num_enemy_total, num_enemy_once, num_beaten, num_generated;
		std::tuple<int, int> schedule_num[7];
		Pipe pipes[7];
		int timers[7];
		EnemyType type;
		char flad_id[8];
	};
};