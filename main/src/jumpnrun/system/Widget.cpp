#include"jumpnrun/system/Widget.h"
#include"utils/string_util.h"
#include<math.h>
using namespace gfw;
using namespace jnr;

Widget::Widget() : x(-1), y(-1), isVisible(true), name("noname"){}
void Widget::run(){}
void Widget::render(BaseMedia& meda){}


StatusBoard::StatusBoard() : myhp(0), myhp_temp(0), mymp(0), mymp_temp(0), myscore(0), type(0)
, enemyhp(0), enemyhp_temp(0), enemymp(0), enemymp_temp(0), message_timer(0), message(""), age(0)
, myhpmax(0), mympmax(0), enemyhpmax(0), enemympmax(0)
, hp_shake_timer(0), mp_shake_timer(0)
{
	name = "statusboard";
	x = 0;
	y = 480 - 96;
}

void StatusBoard::run()
{
	age++;
	const int speed = 1;
	if (message_timer > 0)
		message_timer--;
	if (hp_shake_timer > 0)
		hp_shake_timer--;
	if (mp_shake_timer > 0)
		mp_shake_timer--;

	if (myhp_temp > myhp)
		myhp_temp -= speed;
	if (myhp_temp < myhp)
		myhp_temp += speed;

	if (mymp_temp > mymp)
		mymp_temp -= speed;
	if (mymp_temp < mymp)
		mymp_temp += speed;


	if (enemyhp_temp > enemyhp)
		enemyhp_temp -= speed;
	if (enemyhp_temp < enemyhp)
		enemyhp_temp += speed;

	if (enemymp_temp > enemymp)
		enemymp_temp -= speed;
	if (enemymp_temp < enemymp)
		enemymp_temp += speed;
}
void StatusBoard::setMessage(int time, const std::string& str)
{
	message_timer = time;
	message = str;
}

void StatusBoard::render(BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	//graphics->draw($P.img_statusboard, x, y, 0, 0, );
	//int y = 480 - 96;
	const int NUM_KETA = 7;

	if (message_timer != 0)
	{
		graphics->draw($P.img_statusboard, 0, y, 0, 0, 96 * 0, 640 + 1, 96 + 1, 0, 0xffffffff);
		auto lines = myutil::split(message, "\n");
		for (int i = 0; i < lines.size(); i++)
			graphics->drawString(lines[i], 50, 380 + i * 21, 600, 65);
	}
	else
	{
		const int return_point = 30;
		switch (type)
		{
		case 0:
			graphics->draw($P.img_statusboard, 0, y, 0, 0, 96, 640 + 1, 96 + 1, 0, 0xffffffff);
			//自機HP
			for (int i = 0; i < myhpmax / 5; i++)
			{
				int refx = myhp <= 10 && age % 30 < 15 ? 88 : 0;
				refx = i < myhp_temp / 5 ? refx : 93;
				int i1 = i / return_point;
				int i2 = i % return_point;
				graphics->draw($P.img_statusboard, 
					88 + 5 * (i2 + i1), 
					120 - 96 + y + i1 * 14 + ((hp_shake_timer / 3 % 2)) * 2, 
					0, refx, 288, 5, 13, 0, 0xffffffff);
			}

			//自機MP
			for (int i = 0; i < mympmax / 5; i++)
			{
				int refx = mymp <= 10 && age % 30 < 15 ? 88 : 5;
				refx = i < mymp_temp / 5 ? refx : 93;
				graphics->draw($P.img_statusboard, 
					102 + 5 * i, 
					150 - 96 + y + ((mp_shake_timer / 3 % 2)) * 2,
					0, refx, 288, 5, 13, 0, 0xffffffff);
			}

			//得点
			for (int i = 0; i < NUM_KETA; i++)
			{
				int num = myscore / (int)pow(10.0, NUM_KETA - i - 1) % 10;
				//assert(0 <= num && num < 10);
				if (!(0 <= num && num < 10))
					num = 0;
				graphics->draw($P.img_statusboard, 405 + 8 * i, 162 - 96 + y, 0, 10 + 8 * num, 288, 8, 13, 0, 0xffffffff);
			}

			break;
		case 1:
			graphics->draw($P.img_statusboard, 0, y, 0, 0, 96 * 2, 640 + 1, 96 + 1, 0, 0xffffffff);
			//自機HP
			for (int i = 0; i < myhpmax / 5; i++)
			{
				int refx = myhp <= 10 && age % 30 < 15 ? 88 : 0;
				refx = i < myhp_temp / 5 ? refx : 93;
				int i1 = i / return_point;
				int i2 = i % return_point;
				graphics->draw($P.img_statusboard, 88 + 5 * (i2 + i1), 120 - 96 + y + i1 * 14, 0,
					refx, 288, 5, 13, 0, 0xffffffff);
			}

			//自機MP
			for (int i = 0; i < mympmax / 5; i++)
			{
				int refx = 5;
				refx = i < mymp_temp / 5 ? refx : 93;
				int i1 = i / return_point;
				int i2 = i % return_point;
				graphics->draw($P.img_statusboard, 102 + 5 * i, 150 - 96 + y, 0,
					refx, 288, 5, 13, 0, 0xffffffff);
			}
			//敵HP
			for (int i = 0; i < enemyhpmax / 5; i++)
			{
				int refx = enemyhp <= 10 && age % 30 < 15 ? 88 : 0;
				refx = i < enemyhp_temp /  5 ? refx : 93;
				int i1 = i / return_point;
				int i2 = i % return_point;
				graphics->draw($P.img_statusboard, 534 - 5 * (i1 + i2), 120 - 96 + y + 13 * i1, 0,
					refx, 288, 5, 13, 0, 0xffffffff);
			}

			for (int i = 0; i < enemympmax / 5; i++)
			{
				int refx = 5;
				refx = i < enemymp_temp / 5 ? refx : 93;
				int i1 = i / return_point;
				int i2 = i % return_point;
				graphics->draw($P.img_statusboard, 544 - 5 * i, 150 - 96 + y, 0,
					refx, 288, 5, 13, 0, 0xffffffff);
			}

			break;

		case 2:
			graphics->draw($P.img_statusboard, 0, y, 0, 0, 96 * 0, 640 + 1, 96 + 1, 0, 0xffffffff);
			break;
		}
	}
}