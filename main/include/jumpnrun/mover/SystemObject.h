#pragma once
//#include"Object.h"
#include"jumpnrun/mover/Mover.h"
#include"jumpnrun/stage/Stage.h"
namespace jnr
{
	class ItemPutter: public gfw::Object
	{
	public:
		virtual void initialize();
		virtual void finalize();
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data){}
	private:
		int age;
	};

	class EnemyPutter : public gfw::Object
	{
	public:
		virtual void initialize();
		virtual void finalize();
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data){}
	private:
		int age;
	};
};