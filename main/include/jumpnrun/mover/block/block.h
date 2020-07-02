#pragma once
#include"jumpnrun/mover/character/Character.h"

namespace jnr
{
	class Block : public Character
	{
	public:
		Block(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, double mass = 1.0, int HP = 5, int top_margin = 0);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual void die(jnr::JumpnRunData& data);
		virtual const gfw::VirtualController& Controller();
		virtual gmtr::Vector2D react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up);
		void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		bool is_solid;

	protected:
		gfw::ManualController mc;
	};

	class Trampoline : public Character
	{
	public:
		Trampoline(){}
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual const gfw::VirtualController& Controller();
		virtual gmtr::Vector2D react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up);
		void interact(jnr::Character& chara, jnr::JumpnRunData& data);
	protected:
		gfw::ManualController mc;
		int counter_spring;
	};

	class MovingFloor : public Character
	{
	public:
		MovingFloor(){}
		virtual void initialize(int refx, int refy, gmtr::Vector2D p, gmtr::Vector2D v, int period);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual const gfw::VirtualController& Controller();
		virtual gmtr::Vector2D react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up);
		void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		char flag_id[JNR_FLAG_SIZE];
	protected:
		gfw::ManualController mc;
		int period;
		bool prev_flag;
		gmtr::Vector2D ori_v;
	};

	class DisappearingFloor : public Character
	{
	public:
		DisappearingFloor(){}
		virtual void initialize(int refx, int refy, gmtr::Vector2D p, gmtr::Vector2D v, int period, int delay = 0);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual const gfw::VirtualController& Controller();
		virtual gmtr::Vector2D react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up);
		void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		char flag_id[JNR_FLAG_SIZE];
	protected:
		gfw::ManualController mc;
		int period, delay;
		bool prev_flag;
		bool is_visible;
		gmtr::Vector2D ori_v;
	};

	class FlyingCarpet : public Character
	{
	public:
		enum eVehicleType { Flying, Boat };

		FlyingCarpet(){}
		virtual void initialize(int refx, int refy, gmtr::Vector2D p, gmtr::Vector2D v, int period
			, eVehicleType vehicle_type = Flying);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual const gfw::VirtualController& Controller();
		virtual gmtr::Vector2D react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up);
		void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		char flag_id[JNR_FLAG_SIZE];

	protected:
		gfw::ManualController mc;
		int period;
		bool prev_flag;
		gmtr::Vector2D ori_v;
		bool someone_on_me, moved;
		eVehicleType vehicle_type;
	};

	class Conveyer : public Character
	{
	public:
		Conveyer() {}
		virtual void initialize(int refx, int refy, gmtr::Vector2D p, char rotatio);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual const gfw::VirtualController& Controller();
		virtual gmtr::Vector2D react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up);
		void interact(jnr::Character& chara, jnr::JumpnRunData& data);
		char flag_id[JNR_FLAG_SIZE];
	protected:
		gfw::ManualController mc;
		int period;
		bool prev_flag;
		char rotation;
	};

	class ItemBox: public Character
	{
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner);
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual int damage(int power, int stun_time, int invinc_time);
		virtual int damage(const Damage& d);
		virtual const gfw::VirtualController& Controller();
		virtual void die(jnr::JumpnRunData& data);
		enum ItemBoxType{ Empty, Coin, CommonCard, RareCard };
	protected:
		gfw::ManualController mc;
		int item_type;
	};

	class ItemBlock : public Block
	{
	public:
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner, int top_margin);
		virtual void die(jnr::JumpnRunData& data);
		enum ItemBlockType{ Empty, Coin, CommonCard, RareCard, HPCan, MPCan, Lever};
		int item_type, item_value;
		char flag_id[JNR_FLAG_SIZE];
	protected:
	};

	class Barrel : public Block
	{
	public:
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner);

		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		virtual void run(jnr::JumpnRunData& data);
		//virtual void die(jnr::JumpnRunData& data);
		enum ItemBlockType{ Empty, Coin, CommonCard, RareCard };
		int item_type, item_value;
	protected:
		char rolling;
	};

	class BombBox : public Block
	{
	public:
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner);
		virtual void run(jnr::JumpnRunData& data);
		virtual void die(jnr::JumpnRunData& data);

		char flag_id[JNR_FLAG_SIZE];
	protected:
	};
};