#pragma once
#include"framework/MediaAlternative.h"
#include"jumpnrun/system/Parmanent.h"
#include<vector>
namespace jnr
{
	class Widget
	{
	public:
		Widget();
		virtual void run();
		virtual void render(gfw::BaseMedia& media);
	protected:
		int x, y;
		bool isVisible;
		std::string name;
	};

	class StatusBoard : public Widget
	{
	public:
		StatusBoard();
		void run();
		void render(gfw::BaseMedia& media);
		void setMessage(int time, const std::string& str);

		int myhp, myhp_temp, mymp, mymp_temp, myscore, myhpmax, mympmax;
		int enemyhp, enemyhp_temp, enemymp, enemymp_temp, enemyhpmax, enemympmax;
		char type;

		std::string message;
		int message_timer;
		int age;
		int hp_shake_timer, mp_shake_timer;
	};
};