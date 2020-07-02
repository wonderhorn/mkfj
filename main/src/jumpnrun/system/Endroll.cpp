#include"jumpnrun/system/Endroll.h"
#include"jumpnrun/system/Parmanent.h"
#include"utils/string_util.h"
#include"jumpnrun/system/Title.h"

using namespace gfw;

static const int palyarea_x = 48;
static const int palyarea_y = 78;

static const int palyarea_w = 288;
static const int palyarea_h = 256;

EndrollObject::EndrollObject(int x, int y, int refx,
	int  refy, int  w, int h, int dx, int dy) 
{
	this->x = x;
	this->y = y;
	this->refx = refx;
	this->refy = refy;
	this->w = w;
	this->h = h;
	this->dx = dx;
	this->dy = dy;
	ddx = 0;
	ddy = 0;
	turned = false;

	scale = 1.0;
	dscale = 0.0;

	this->anim_counter = 0;
	this->anim_num = 1;
	this->anim_interval = 1;
}

void EndrollObject::setAnimation(int anim_num, int anim_interval) 
{
	this->anim_counter = 0;
	this->anim_num = anim_num;
	this->anim_interval = anim_interval;
}


bool Endroll::initialize()
{
	age = 000;
	//age = 1800;
	background = nullptr;

	texts = 
	{
		"",
		"",
		"まかふじ！　",
		"スタッフロール",
		"",
		"",
		"企画",
		"ディレクション",
		"プログラミング",
		"デザイン",
		"ミュージック",
		"  WonderHorn",
		"",
		"",
		"",
		"テストプレイ",
		"  （現在募集中）",
		"",
		"",
		"",
		"フリー素材",
		"",
		"  効果音ラボ 様",
		"  https://soundeffect-lab.inf",
		"",
		"  フリー効果音 様",
		"  https://taira-komori.jpn.org",
		"",
		"  魔王魂 様",
		"  https://maoudamashii.jokersounds.com",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
	};
	return true;
}

void Endroll::play(int t, std::map<std::string, EndrollObject>& objects)
{
	static int ground_height = 32;
	switch (t) 
	{
	case 0:
		ground_height = 32;
		objects.clear();
		objects["fjtb"] = EndrollObject(110, palyarea_h - ground_height - 64, 0, 0, 64, 64, 0, 0);
		objects["fjtb"].setAnimation(2, 35);
		objects["volly"] = EndrollObject(15, palyarea_h - ground_height - 64, 0, 64, 32, 64, 0, 0);
		objects["wzrk"] = EndrollObject(215, palyarea_h - ground_height - 64, 64, 64, 32, 64, 0, 0);
		for (int x = 0; x < palyarea_w; x += 64)
		{
			mapchips[std::to_string(x)] = EndrollObject(x, palyarea_h - ground_height, 0, 0, 64, 64, 0, 0);
		}
		background = &$P.img_school;
		bgx = -60;
		bgy = -140;
		break;
	case 120 + 60:
		objects["volly"] = EndrollObject(15, palyarea_h - ground_height - 64, 32, 64, 32, 64, 0, 0);
		objects["ball"] = EndrollObject(20, palyarea_h - ground_height - 64 + 10, 256, 0, 32, 32, 3, -6);
		objects["ball"].ddy = 0.2;
		break;
	case 120 + 80:
		objects["volly"] = EndrollObject(15, palyarea_h - ground_height - 64, 0, 64, 32, 64, 0, 0);
		break;
	case 120 + 85:
		//objects["ball"].dy = 3;
		break;
	case 120 + 110:
		objects["wzrk"] = EndrollObject(215, palyarea_h - ground_height - 64, 64 + 32, 64, 32, 64, 0, 0);
		break;
	case 120 + 120:
		objects["wzrk"] = EndrollObject(215, palyarea_h - ground_height - 64, 64 + 32*2, 64, 32, 64, 0, 0);
		objects["ball"].dx = -3;
		objects["ball"].dy = -6;
		objects["ball"].ddy = 0.2;
		//objects["ball"].dy = 3;
		break;
	case 120 + 130:
		objects["wzrk"] = EndrollObject(215, palyarea_h - ground_height - 64, 64 + 32 * 0, 64, 32, 64, 0, 0);
		break;
	case 120 + 180:
		objects["volly"] = EndrollObject(15, palyarea_h - ground_height - 64, 32, 64, 32, 64, 0, 0);
		objects["ball"].dx = 3;
		objects["ball"].dy = -6;
		objects["ball"].ddy = 0.2;
		//objects["ball"].dy = 3;
		break;
	case 120 + 200:
		objects["volly"] = EndrollObject(15, palyarea_h - ground_height - 64, 0, 64, 32, 64, 0, 0);
		break;
	case 120 + 230:
		objects["wzrk"] = EndrollObject(215, palyarea_h - ground_height - 64, 64 + 32, 64, 32, 64, 0, 0);
		break;
	case 120 + 240:
		objects["wzrk"] = EndrollObject(215, palyarea_h - ground_height - 64, 64 + 32 * 2, 64, 32, 64, 0, 0);
		objects["ball"].dx = -2;
		objects["ball"].dy = -3;
		objects["ball"].ddy = 0.2;
		//objects["ball"].dy = 3;
		break;
	case 120 + 250:
		objects["wzrk"] = EndrollObject(215, palyarea_h - ground_height - 64, 64 + 32 * 0, 64, 32, 64, 0, 0);
		break;
	case 120 + 273:
		objects["ball"].dx = -2;
		objects["ball"].dy = -3;
		objects["ball"].ddy = 0.2;
		objects["fjtb"] = EndrollObject(110, palyarea_h - ground_height - 64, 64 * 3, 0, 64, 64, 0, 0);

		break;
	case 120 + 333:
		objects["fjtb"] = EndrollObject(110, palyarea_h - ground_height - 64, 64 * 2, 0, 64, 64, 0, 0);
		break;
	case 120 + 403:
		objects["fjtb"] = EndrollObject(110, palyarea_h - ground_height - 64, 0, 0, 64, 64, 0, 0);
		objects["fjtb"].setAnimation(2, 35);
		break;
	case 120 + 453:
		objects["wzrk"] = EndrollObject(215, palyarea_h - ground_height - 64, 64 + 32 * 3, 64, 32, 64, 0, 0);

		break;

	case 600:
		objects.clear();
		background = &$P.img_forest;
		bgx = -60;
		bgy = -60;
		objects["fjtb"] = EndrollObject(70, palyarea_h - ground_height - 64, 0, 64*3, 32, 64, 0, 0);
		mapchips.clear();
		for (int x = 0; x < palyarea_w; x += 64)
		{
			mapchips[std::to_string(x)] = EndrollObject(x, palyarea_h -ground_height, 64 * 9, 0, 64, 64, 0, 0);
		}
		break;
	case 601:
		break;
	case 660:
		objects["fjtb"].turned = true;
		break;
	case 690:
		objects["fjtb"].turned = false;
		break;
	case 750:
		objects["tktm"] = EndrollObject(180, -64, 32 * 2, 64 * 2, 32, 64, 0, 8);
		break;
	case 779:
		objects["tktm"] = EndrollObject(180, palyarea_h - ground_height - 64, 0, 64 * 2, 32, 64, 0, 0);
		objects["tktm"].dy = 0;
		objects["fjtb"] = EndrollObject(70, palyarea_h - ground_height - 64, 32 * 4, 64 * 3, 32, 64, -2, 0);
		break;
	case 784:
		objects["fjtb"].dx = 0;
		break;
	case 880:
		objects["tktm"] = EndrollObject(180, palyarea_h - ground_height - 64, 32 * 3, 64 * 2, 32, 64, -1, 0);
		objects["tktm"].setAnimation(2, 20);
		objects["fjtb"].dx = -1;

		break;
	case 900:
		objects["fjtb"].dx = 0;
		objects["tktm"].refx = 0;
		objects["tktm"].setAnimation(1, 1);
		objects["tktm"].dx = 0;
		objects["frg"] = EndrollObject(objects["tktm"].x + 3, -32, 32 * 6, 64 * 1, 32, 32, 0, 8);
		objects["frg"].turned = true;
		break;
	case 921:
		objects["tktm"].refx = 32 * 5;
		objects["frg"].y -= 1;
		objects["frg"].dy = 0;
		objects["frg"].setAnimation(2, 40);
		break;
	case 980:
		objects["tktm"].refx = 0;
		objects["frg"].y -= 1;
		break;
	case 1020:
		objects["tktm"].refx = 32 * 1;
		objects["fjtb"].refx = 0;
		break;


	case 1200:
		objects.clear();
		mapchips.clear();

		ground_height = 92;
		background = &$P.img_river;
		for (int x = 0; x < palyarea_w; x += 64)
		{
			mapchips[std::to_string(x) + "_1"] = EndrollObject(x, palyarea_h - ground_height + 64, 64 * 3, 64 * 4, 64, 64, 0, 0);
		}
		for (int x = 0; x < palyarea_w; x += 64)
		{
			mapchips[std::to_string(x) + "_2"] = EndrollObject(x, palyarea_h - ground_height,
				64 * 3, 64 * 3, 64, 64, 0, 0);
		}
		for (int x = 64*3; x < palyarea_w; x += 64)
		{
			mapchips[std::to_string(x) + "_1"] = EndrollObject(x, palyarea_h - ground_height + 64, 
				64 * 0, 64 * 1, 64, 64, 0, 0);
		}
		for (int x = 64 * 3; x < palyarea_w; x += 64)
		{
			mapchips[std::to_string(x) + "_2"] = EndrollObject(x, palyarea_h - ground_height,
				64 * 0, 64 * 1, 64, 64, 0, 0);
		}
		objects["wkb"] = EndrollObject(64 * 3 - 24, palyarea_h - ground_height - 53,
			0, 64 * 4, 64, 64, 0, 0);
		objects["fish"] = EndrollObject(-32, palyarea_h - ground_height + 16, 
			32 * 6, 64 * 1 + 32, 32, 32, 2, 0);
		objects["fish"].setAnimation(2, 20);
		objects["fjtb"] = EndrollObject(64 * 3 - 56, palyarea_h + 120, 
			32 * 5, 64 * 3, 32, 64, 0, -1);
		break;
	
	case 1296:
		objects["fish"].turned = true;
		objects["fish"].dx = -2;
		break;
	case 1380:
		objects["wkb"].refx = 64;
		objects["fjtb"].dy = 0;
		break;
	case 1395:
		objects["wkb"].refx = 0;
		break;
	case 1410:
		objects["wkb"].refx = 64;
		break;
	case 1440:
		objects["wkb"].refx = 64 * 2;
		objects["fjtb"].dy = -20;
		objects["fjtb"].dx = 2.5;
		objects["fjtb"].ddy = 1;
		break;
	case 1476:
		objects["fjtb"].dy = 0;
		objects["fjtb"].dx = 0;
		objects["fjtb"].ddy = 0;
		objects["fjtb"].refx = 32 * 6;
		objects["fjtb"].w = 64;
		objects["fjtb"].y = palyarea_h - ground_height - 64;
		break;
	case 1530:
		objects["wkb"].refx = 64 * 0;
		break;
	case 1535:
		objects["fish"] = EndrollObject(-32, palyarea_h - ground_height + 24,
			32 * 6, 64 * 1 + 32, 32, 32, 2, 0);
		objects["fish"].setAnimation(2, 20);
		break;
	case 1627:
		objects["fish"].turned = true;
		objects["fish"].dx = -2;
		break;

	case 1800:
		ground_height = 32;
		objects.clear();
		mapchips.clear();
		for (int y = 0; y < palyarea_h; y += 64)
			for (int x = 0; x < palyarea_w; x += 64)
		{
			mapchips[std::to_string(x) + "_" + std::to_string(y) + "wall"] = 
				EndrollObject(x, y, 64 * 7, 64 * 6, 64, 64, 0, 0);
		}
		for (int x = 0; x < palyarea_w; x += 64)
		{
			mapchips[std::to_string(x) + "celling"] = 
				EndrollObject(x, palyarea_h - ground_height - 64 * 3 - 32, 0, 64, 64, 64, 0, 0);
			mapchips[std::to_string(x) + "floor"] = 
				EndrollObject(x, palyarea_h - ground_height, 64 * 5, 64 * 9, 64, 64, 0, 0);
		}

		for (int x = 32; x < palyarea_w - 32; x += 64)
		{
			objects["mgr" + std::to_string(x)] = 
				EndrollObject(x, palyarea_h - ground_height - 20, 384, 96, 32, 32, 0, 0);

		}
		objects["fjtb"] = EndrollObject(16, palyarea_h - ground_height - 64, 0, 64 * 3, 32, 64, 0, 0);
		objects["krmr"] = EndrollObject(250, palyarea_h - ground_height - 64, 320, 64 * 2, 32, 64, 0, 0);
		objects["krmr"].turned = true;
		objects["mole"] = EndrollObject(-32, palyarea_h - ground_height - 32 + 7, 384 + 32, 96, 32, 32, 0, 0);
		break;
	case 1800 + 60:
		objects["mole"].x = 32 + 64 * 1;
		break;
	case 1800 + 120:
		objects["krmr"].refx += 32;
		objects["krmr"].setAnimation(2, 35);
		break;
	case 1800 + 130:
		objects["mole"].x = -32;
		break;
	case 1800 + 140:
		objects["blt"] = EndrollObject(objects["krmr"].x, objects["krmr"].y + 16,
			384 + 64, 96, 32, 32, -12, 0);
		objects["blt"].turned = true;
		break;
	case 1800 + 160:
		objects["krmr"].refx -= 32;
		objects["krmr"].setAnimation(1, 35);
		break;
	case 1800 + 165 - 4:
		objects["fjtb"].refx = 32 * 10;
		objects["fjtb"].w = 64;
		objects["fjtb"].x -= 16;
		objects["fjtb"].dx = -2;
		objects["blt"].x = -100;
		break;
	case 1800 + 165 + 2 - 4:
		objects["fjtb"].dx = 0;
		break;
	case 1800 + 165 + 30:
		objects["fjtb"].refx = 0;
		objects["fjtb"].w = 32;
		objects["fjtb"].x += 16;
		break;
	case 1800 + 240:
		objects["mole"].x = 32 + 64 * 2;
		break;
	case 1800 + 260:
		objects["krmr"].refx += 32;
		objects["krmr"].setAnimation(2, 35);
		break;
	case 1800 + 270:
		objects["mole"].x = -32;
		break;
	case 1800 + 280:
		objects["blt"] = EndrollObject(objects["krmr"].x, objects["krmr"].y + 16,
			384 + 64, 96, 32, 32, -12, 0);
		objects["blt"].turned = true;
		break;
	case 1800 + 300:
		objects["krmr"].refx -= 32;
		objects["krmr"].setAnimation(1, 35);
		break;
	case 1800 + 305:
		objects["fjtb"].refx = 32 * 10;
		objects["fjtb"].w = 64;
		objects["fjtb"].x -= 16;
		objects["fjtb"].dx = -2;
		objects["blt"].x = -100;
		break;
	case 1800 + 305 + 2:
		objects["fjtb"].dx = 0;
		break;
	case 1800 + 305 + 30:
		objects["fjtb"].refx = 0;
		objects["fjtb"].w = 32;
		objects["fjtb"].x += 16;
		break;
	case 1800 + 405:
		objects["anger"] = EndrollObject(objects["fjtb"].x - 6, objects["fjtb"].y - 6, 
			256, 64, 32, 32, 0, 0);
		break;
	case 1800 + 450:
		objects["fjtb"].refx = 8 * 32;
		objects["fjtb"].setAnimation(2, 35);
		break;
	case 1800 + 499 - 4:
		objects["ball"] = EndrollObject(objects["fjtb"].x, palyarea_h - ground_height - 64 + 10, 
			32 * 9, 0, 32, 32, 4, -5);
		objects["ball"].dx = 4.5;
		objects["ball"].ddy = 0.2;
		break;
	case 1800 + 504:
		objects["fjtb"].refx = 0 * 32;
		objects["fjtb"].setAnimation(1, 35);
		break;
	case 1800 + 532 + 9:
		objects["krmr"].refx += 32 * 3;
		objects["krmr"].dx = 6;
		objects["krmr"].dy = -4;
		objects["zexp"] = EndrollObject(objects["ball"].x, objects["ball"].y, 
			32 * 9, 32, 32, 32, 0, 0);
		objects["zexp"].dscale = 0.4;
		objects["ball"].x = -1100;
		break;
	case 1800 + 538 + 10 + 9:
		objects["zexp"].x = -100;
		objects["zexp"].scale = 0;
		objects["zexp"].dscale = 0;
		break;

	case 2400:
		objects.clear();
		ground_height = 56;
		objects["fjtb"] = EndrollObject(140, palyarea_h - ground_height - 64, 384, 0, 64, 64, 0, 0);
		objects["fjtb"].setAnimation(2, 100);
		objects["hrmk"] = EndrollObject(140 + 8, palyarea_h - ground_height - 64 + 1, 512, 0, 64, 64, 0, 0);
		objects["window"] = EndrollObject(170, palyarea_h - ground_height - 96, 320, 64, 64, 64, 0, 0);
		objects["desk"] = EndrollObject(40, palyarea_h - ground_height - 64, 320, 0, 64, 64, 0, 0);
		mapchips.clear();
		for (int x = 0; x < palyarea_w; x += 64)
		{
			mapchips[std::to_string(x) + "celling"] = EndrollObject(x, palyarea_h - ground_height - 64 * 3 -32, 64, 64*5, 64, 64, 0, 0);
			mapchips[std::to_string(x)] = EndrollObject(x, palyarea_h - ground_height, 0, 0, 64, 64, 0, 0);
		}
		background = &$P.img_room;
		bgx = -60;
		bgy = -140;
		break;
	case 2400 + 300 - 90:
		objects["hrmk"].refx +=64;
		break;

	case 2400 + 300:
		objects["zexp"] = EndrollObject(170 + 56 - 16, palyarea_h - ground_height - 96 + 8, 384, 64, 16, 16, 0, 0);
		break;
	case 2400 + 303:
		objects["zexp"].x = -100;
		objects["zstar"] = EndrollObject(170 + 56 - 16, palyarea_h - ground_height - 96 + 8, 384 + 16, 64, 16, 16, -2, 1);
		break;
	case 2400 + 323:
		objects["zstar"].x = -100;
		break;
	case 2400 + 323 + 120:
		objects["hrmk"].refx -= 64;
		break;
	/*
	case 3000:
		objects.clear();
		mapchips.clear();
		background = &$P.img_paper;
		break;
	*/
	default:
		break;
	}

	
}

void Endroll::finalize()
{}

Screen::eScreenState Endroll::run(gfw::BaseMedia& media) 
{
	if (age == 1) 
	{
		$P.media.musics.resetBGM();
		$P.media.musics.setAutoRepeat(false);
		$P.media.musics.startBGM(std::string("bgm\\end.mp3"));
	}
	
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		auto& obj = it->second;
		obj.x += obj.dx;
		obj.y += obj.dy;
		obj.dx += obj.ddx;
		obj.dy += obj.ddy;
		obj.scale += obj.dscale;
		obj.anim_counter += 1;
	}

	play(age, objects);
	age++;

	if (age >= 3600)
	{
		Title* title = new Title(game);
		title->initialize();
		game->push_back(title);
		return eScreenState::Finished;
	}
	return eScreenState::Continuing;

}
void Endroll::render(gfw::BaseMedia& media) 
{
	BaseGraphics* graphics = media.getGraphics();
	if(background)
		graphics->draw(*background, palyarea_x + bgx, palyarea_y + bgy, 0,
			0, 0, 640, 480, false, 0xffffffff);
	for (auto it = mapchips.begin(); it != mapchips.end(); it++)
	{
		auto& obj = it->second;
		graphics->draw(
			$P.img_map,
			obj.x + palyarea_x,
			obj.y + palyarea_y,
			0,
			obj.refx + ((obj.anim_counter / obj.anim_interval) % obj.anim_num) * obj.w,
			obj.refy,
			obj.w, obj.h, obj.turned, 0xffffffff);

	}
	for (auto it = objects.begin(); it != objects.end(); it++) 
	{
		auto& obj = it->second;
		int sign = 1 - 2 * obj.turned;
		graphics->drawSpinResize(
			$P.img_endrollobj, 
			obj.x + palyarea_x, 
			obj.y + palyarea_y, 
			0,
			obj.refx + ((obj.anim_counter / obj.anim_interval) % obj.anim_num) * obj.w, 
			obj.refy, 
			obj.w, obj.h, obj.turned, 0xffffffff, 
			sign * obj.scale, obj.scale, 0);

	}

	if (age < 135 + 255 / 2)
	{
		int brightness = 255 - max(0, age - 135) * 2;
		brightness = max(0, brightness);
		graphics->clear(0, 0, 0, brightness);
	}
	if (age >= 3000 - 90) 
	{
		int brightness = 255 * (age - (3000 - 90)) / 180;
		brightness = min(255, brightness);
		graphics->clear(0, 0, 0, brightness);
	}
	if (age >= 3000 + 210) 
	{
		graphics->drawString("THE END"
			, palyarea_x + 115, palyarea_y + 90, 200, 50
			, 1.0, 1.0, 1.0);
	}
	if (age >= 3000 + 210 + 90)
	{
		graphics->drawString("Thank you for playing!"
			, palyarea_x + 115 - 42, palyarea_y + 90 + 45, 200, 50
			, 1.0, 1.0, 1.0);
	}


	graphics->draw($P.img_endrollpaper, 0, 0, 0, 0, 0, 640, 480, false, 0xffffffff);

	for (int i = 0; i < texts.size(); i++)
	{
		graphics->drawString(texts[i]
			, 380, 128 + i * 40 + 300 - (age / 2), 200, 50);
	}

	if (age < 255 / 2 + 45)
	{
		int brightness = 255 - max(0, age - 45) * 2;
		brightness = max(0, brightness);
		graphics->clear(0, 0, 0, brightness);
	}
	if (age >= 3000 + 340)
	{
		int brightness = 255 * max(0, age - (3000 + 340)) / 180;
		brightness = min(255, brightness);
		graphics->clear(255, 255, 255, brightness);
	}

}