#pragma once
#include"jumpnrun/mover/bullet/Bullet.h"
#include"framework/GameUtil.h"

namespace jnr
{
	class Item: public Bullet
	{
	public:
		Item(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v
			, double mass, const std::string& flag_id = "");
		virtual void run(jnr::JumpnRunData& data);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);

		char flag_id[JNR_FLAG_SIZE];

		int hp_gain, mp_gain, score_gain;
		bool is_silent;
		int timer_show_down_button;
	protected:
	};

	class HPCan : public Item
	{
	public:
		HPCan(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, double mass, const std::string& flag_id = "");
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
	};
	class MPCan : public Item
	{
	public:
		MPCan(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, double mass, const std::string& flag_id = "");
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
	};

	class DeckReset : public Item
	{
	public:
		DeckReset(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int power, gmtr::Vector2D v, double mass);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
	private:
		gfw::Timer timer_inactive;
	};

	class Gacha : public Item
	{
	public:
		Gacha() {}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, double mass);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
	private:
		gfw::Timer timer_inactive;
	};

	class NextStage : public Item
	{
	public:
		NextStage(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, double mass
			, const std::string& stage_name = "", int start_x = 0, int start_y = 0);
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		int stage_change_id, start_x, start_y;
		char stage_name[64];
	};

	class EncountBoss : public Item
	{
	public:
		EncountBoss(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, double mass
			, int boss_id = 0, const std::string& flag_id = "", const std::string& flag_id_activate="");
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data){}
		int boss_id;
		char flag_id_activate[JNR_FLAG_SIZE];
	};

	class EventCaller : public Item
	{
	public:
		EventCaller() {}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, double mass
			, const std::string& flag_id = "");
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data) {}
		char flag_id[JNR_FLAG_SIZE];

	};

	class GetCard : public Item
	{
	public:
		GetCard(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, double mass, int card_id = 0, const std::string& flag_id = "");
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		int card_id;
	};
};