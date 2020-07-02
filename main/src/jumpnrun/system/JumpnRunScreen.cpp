#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include <stdlib.h>
#include <crtdbg.h>

#include"jumpnrun/system/JumpnRunScreen.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/mover/Enchant.h"
#include"jumpnrun/mover/enemy/Enemy.h"
#include"jumpnrun/mover/enemy/PunchingMachine.h"
#include"jumpnrun/mover/block/block.h"
#include"jumpnrun/mover/bullet/Bubble.h"
#include"jumpnrun/mover/Item.h"
#include"jumpnrun/mover/block/Switch.h"
#include"jumpnrun/mover/enemy/EnemyGroup.h"
#include"jumpnrun/mover/SystemObject.h"
#include"jumpnrun/GRAPHICS.h"
#include"adv/AdventureScreen.h"
#include"jumpnrun/system/Title.h"
#include"jumpnrun/system/Ending.h"
#include"jumpnrun/system/Endroll.h"
#include"jumpnrun/stage/StageSelectionScreen.h"
#include"jumpnrun/spell/CardGalleryScreen.h"
#include"jumpnrun/spell/DeckEditScreen.h"
#include"jumpnrun/spell/GachaponScreen.h"
#include"jumpnrun/system/ConfigScreen.h"
#include"jumpnrun/system/Achievement.h"
#include"jumpnrun/stage/StageDataLoader.h"
#include"jumpnrun/system/BossFactory.h"
#include"utils/string_util.h"
#include<iostream>

using namespace gfw;
using namespace jnr;
using std::max;
using std::min;
#define $V gmtr::Vector2D

JumpnRunScreen::JumpnRunScreen(gfw::Game* game)
	: Screen(game), counter(0), ss(*$P.players[0].vc, &$P.players[0].deck_tmp)
{}
JumpnRunScreen::~JumpnRunScreen(){}

bool JumpnRunScreen::initialize()
{
	//for (auto i : *MoverClassRegisterer::entries)
	//{
	//	std::cout << i.first << std::endl;
	//}

	$P.jnr_scr = this;

	paused = false;
	timer_return_from_boss = timer_start_boss
		= timer_restart = timer_lastbattle = -1;
	boss_beaten = false;
	boss_encounted = false;
	$P.camera.isFixed = false;

	bool result;
	result = tl.initialize();
	objmng = gfw::ObjectManager(&tl);
	//stg.initializeTestStage();
	this->data.pullFromParmanent();

	if (data.next_stage_name.size() > 0)
	{
		if ($P.flags["prolog"] && !$P.flags["prolog2"]
			&& data.next_stage_name == "classroom1.txt")
		{
			data.next_stage_name = "classroom1_prolog.txt";
		}
		StageDataLoader().load("map\\" + data.next_stage_name, &tl, stg, objmng, &bg);
		stg.StartPoint($P.next_start_p);
		$P.start_bgm = false;
		if ($P.reset_bgm)
			$P.media.musics.stopBGM();
	}
	else StageDataLoader().load("map\\classroom_start_prolog.txt", &tl, stg, objmng, &bg);

	Mover::stg = &stg;
	Object::tl = &tl;
	Player* p = generateMover(Player, &tl);
	p->initialize(0, 0, stg.StartPoint() + $V(0, -0.5), 0, 0);
	//p->initialize(REFPOINT_BOSS3_X, REFPOINT_BOSS3_Y, stg.StartPoint(), 0, 0);
	player = Object::Pipe(p, tl);
	p->HP() = $P.players[0].hp_current;
	p->HPMax() = $P.players[0].hp_max;
	p->MP() = $P.players[0].mp_current;
	p->MPMax() = $P.players[0].mp_max;
	p->Score() = $P.players[0].score;
	p->Turned($P.turned);
	if ($P.getFlag("pgbck") && 
		(
			data.next_stage_name == "mansion4.txt" ||
			data.next_stage_name == "mansion5.txt" ||
			data.next_stage_name == "mansion6.txt"
			))
	{
		p->piggyBack(true);
		p->SpeedDash(1.75);
		p->SpeedWalk(0.65);
		p->SpeedJump(5.25);
	}
	data.camera.setMax(stg.Width(), stg.Height());
	data.camera.setPos(p->P().x - 320, p->P().y - 200);
	objmng.setActiveWindow(data.camera.X(), data.camera.Y(), 640, 480);
	objmng.generateNear();
	data.in_boss_battle = false;
	/*generate<AIPlayer>(tl)->initialize
		(REFPOINT_ME_X, REFPOINT_ME_Y, $V(0, 0), 2, &$P.ai);
		generate<Item>(tl)->initialize(REFPOINT_JEWEL_X, REFPOINT_JEWEL_Y + 32, $V(64*6, 0), -1, 10, $V(0, 0), 1);*/
	//generate<ItemPutter>(tl)->initialize();
	//generate<EnemyPutter>(tl)->initialize();
	//tl.reset([](void* ptr){});
	//generate<HPCan>(tl)->initialize(REFPOINT_HPCAN_X, REFPOINT_HPCAN_Y, $V(64 * 3, 64 * 5), -1, 1);
	//generate<enm::Boy>(tl)->initialize(REFPOINT_GIRL1_X, REFPOINT_GIRL1_Y, $V(64 * 3, 64 * 5), 1);
	//generate<enm::Kendo>(tl)->initialize(REFPOINT_KENDO_X, REFPOINT_KENDO_Y, $V(64 * 6, 0), 1);
	//generate<enm::Snail>(tl)->initialize(REFPOINT_SNAIL_X, REFPOINT_SNAIL_Y, $V(64 * 6, 64 * 3), 1);
	//generate<enm::Bicycle>(tl)->initialize(REFPOINT_BICYCLE_X, REFPOINT_BICYCLE_Y, $V(64 * 6, 64 * 3), 1);
	//generate<enm::Pylonman>(tl)->initialize(REFPOINT_PYLONMAN_X, REFPOINT_PYLONMAN_Y, $V(64 * 4, 64 * 3), 1, 1);
	//generate<enm::Elf>(tl)->initialize(REFPOINT_ELF_X, REFPOINT_ELF_Y, $V(64 * 4, 64 * 3), 1, 1);
	//generate<enm::Mandragora>(tl)->initialize(REFPOINT_MANDRAKE_X, REFPOINT_MANDRAKE_Y, $V(64 * 4, 64 * 3), 1, 1);
	//generate<enm::Bug>(tl)->initialize(REFPOINT_BUG_BLACK_X, REFPOINT_BUG_BLACK_Y, $V(64 * 4, 64 * 3), 1, 1);
	//generate<enm::BigEye>(tl)->initialize($V(64 * 4, 64 * 3), 1, 1);
	//generate<enm::Bird>(tl)->initialize(REFPOINT_BIRD_X, REFPOINT_BIRD_Y, $V(64 * 4, 64 * 3), 1, 1);
	//generate<enm::JellyFish>(tl)->initialize(REFPOINT_JELLYFISH_X, REFPOINT_JELLYFISH_Y, $V(64 * 4, 64 * 5), 1, 1);
	//generate<enm::Fish>(tl)->initialize(REFPOINT_FISH_X, REFPOINT_FISH_Y, $V(64 * 4, 64 * 5), 1, 1);
	//generate<enm::Fairy>(tl)->initialize(REFPOINT_FAIRY_BLUE_X, REFPOINT_FAIRY_BLUE_Y, $V(64 * 4, 64 * 5), 1, 1);
	//generate<enm::Knight>(tl)->initialize(REFPOINT_KNIGHT_X, REFPOINT_KNIGHT_Y, $V(64 * 4, 64 * 3), 1, 1);
	/*generate<enm::Slime>(tl)->initialize(REFPOINT_SLIME_X, REFPOINT_SLIME_Y, $V(64 * 4, 64 * 3), 1, 1);
	generate<Switch>(tl)->initialize(REFPOINT_TOGGLESWITCH_X, REFPOINT_TOGGLESWITCH_Y, $V(64 * 5, 64 * 3), -1);
	generate<DeckReset>(tl)->initialize(REFPOINT_BOOK_X, REFPOINT_BOOK_Y, $V(64 * 6, 64 * 3), 0, 0, $V(0, 0), 1.0);
	generate<NextStage>(tl)->initialize(REFPOINT_START_X, REFPOINT_START_Y, $V(64 * 7, 64 * 3), 0, 1.0);
	generate<EncountBoss>(tl)->initialize(REFPOINT_START_X, REFPOINT_START_Y, $V(64 * 8, 64 * 3), 0, 1.0);*/
	//generate<Barrel>(tl)->initialize(REFPOINT_BARREL_X, REFPOINT_BARREL_Y, $V(64 * 4, 64 * 3), -1);
	//generate<Barrel>(tl)->initialize(REFPOINT_BARREL_X, REFPOINT_BARREL_Y, $V(64 * 4, 64 * 3+32), -1);
	//generate<Barrel>(tl)->initialize(REFPOINT_BARREL_X, REFPOINT_BARREL_Y, $V(64 * 4, 64 * 3+64), -1);
	//generate<Trampoline>(tl)->initialize(REFPOINT_SPRING_X, REFPOINT_SPRING_Y, $V(64 * 2, 64 * 4), -1);
	//generate<Block>(tl)->initialize(REFPOINT_BLOCK_X, REFPOINT_BLOCK_Y, $V(64 * 4, 64 * 3), -1);
	//generate<Block>(tl)->initialize(REFPOINT_BLOCK_X, REFPOINT_BLOCK_Y, $V(64 * 4, 64 * 4), -1);
	//generate<MovingFloor>(tl)->initialize(REFPOINT_STEP_X, REFPOINT_STEP_Y, $V(64 * 3, 64 * 5), $V(0, -1), 128);
	//generate<MovingFloor>(tl)->initialize(REFPOINT_STEP_X, REFPOINT_STEP_Y, $V(64 * 5, 64 * 2), $V(0, 1), 128);
	//generate<FlyingCarpet>(tl)->initialize(REFPOINT_CARPET_X, REFPOINT_CARPET_Y, $V(64 * 3, 64 * 25), $V(0, 0), 128);
	//generate<enm::Mushroom>(tl)->initialize(REFPOINT_MUSHROOM_X, REFPOINT_MUSHROOM_Y, $V(64 * 6, 64 * 3), 1);
	//generate<enm::BigSlime>(tl)->initialize(REFPOINT_SLIME_X, REFPOINT_SLIME_Y, $V(64 * 6, 64 * 3), 1);
	//generate<GetCard>(tl)->initialize(REFPOINT_CARD_X, REFPOINT_CARD_Y, $V(64 * 4, 64 * 3), 0, 1.0);
	//generate<Ball>(tl)->initialize(REFPOINT_VOLLEYBALL_X, REFPOINT_VOLLEYBALL_Y, $V(64 * 3, 64 * 5), $V(0, 0), 1, -1);
	//generate<EmitFire>(tl)->initialize($V(64 * 2, 64 * 8), $V(0, -4), -1, 1);
	//generate<enm::PunchingMachine>(tl)->initialize($V(64 * 2, 64 * 0), -1);
	/*EnemyGroup* enmg = generate<EnemyGroup>(tl);
	enmg->initialize($V(64 * 4, 64 * 4), 1);
	boss = Object::Pipe(enmg, tl);*/
	/*Switch* sw = generate<Switch>(tl);
	sw->initialize(REFPOINT_TOGGLESWITCH_X, REFPOINT_TOGGLESWITCH_Y, $V(64 * 5, 64 * 3), -1);
	Door* door = generate<Door>(tl);
	door->initialize(REFPOINT_SHUTTER_X, REFPOINT_SHUTTER_Y, $V(64 * 7, 64 * 3), -1);
	sw->setReciever(door);*/
	/*generate<enm::TrapTimered>(tl)->initialize(REFPOINT_TRAP_X, REFPOINT_TRAP_Y
		, $V(64 * 4, 64 * 3), -1);*/
	//generate<Gacha>(tl)->initialize(REFPOINT_GACHA_X, REFPOINT_GACHA_Y, $V(64*3, 64*6), -1, 1.0);

	if (!$P.flags["prolog"])
	{
		generate<PrologFujitsubo>(tl)->initialize();
		p->Visible(false);
	}

	if ($P.flags["prolog"] && !$P.flags["prolog2"]
		&& data.next_stage_name == "classroom1_prolog.txt")
	{
		generate<PrologHand>(tl)->initialize();
		data.next_stage_name = "classroom1.txt";
		//p->Visible(false);
	}

	$V myp = $V(320, 240);
	if (player.Dst(tl))
	{
		Player* p = (Player*)player.Dst(tl);
		myp = p->P();
	}
	controls.push_back(
		new START_STAGE(myp.x + 16 - data.camera.X(),
			myp.y + 16 - data.camera.Y()));

	ss.setDeck(&data.players[0].deck_tmp);
	ss.SetCursor($P.deck_cursor);
	//$P.players[0].deck.shuffle();
	//data.players[0].deck.shuffle();
	//$P.replaycont.data = ReplayController::test_data();
	//$P.replaycont.readFile("replay\\1449794866.txt");
	//$P.recorder.clear();

	/*$P.mutex_rand.lock();
	$P.rand.reset(123456789);
	$P.mutex_rand.unlock();*/

	//リセットするフラグ類
	$P.flags["nbossb6"] = $P.flags["nbossb7"];
	$P.flags["gameclear"] = 0;
	$P.flags["lastbtl"] = 0;
	$P.flags["dropcdl"] = 0;
	$P.flags["setcdl"] = 0;
	$P.flags["dspkyo"] = ($P.flags["nbossb6"] != 0); //stg5cだとクリア後に戻らなかった
	$P.flags["br5cls"] = ($P.flags["nbossb6"] == 0 || $P.flags["nbossb7"] == 0);

	$P.turn_boss_dummy = false;

	// update some times to avoid awkward movements in the firs frame
	data.players[0].vc->force_to_release = true;
	for(int i = 0; i < 5; i++)
		updateObjects();
	data.players[0].vc->force_to_release = false;

	return result;
}

void JumpnRunScreen::finalize()
{
	unsigned int t = time(nullptr);
	std::string str = myutil::int2str(t);
	if($P.to_save)
		$P.save($P.save_file_name);
	//$P.recorder.writeFile(std::string("replay\\") + str + ".txt");
	//$P.recorder.clear();

	//dumpTaskListToFile(str + ".dat");

	data.encount_boss = -1;
	data.players[0].max_score = max(data.players[0].max_score, data.players[0].score);
	data.players[0].score = 0;
	data.pushToParmanent();

	while(!controls.empty())
	{
		auto ctrl = controls.front();
		delete ctrl;
		controls.pop_front();
	}
}

void JumpnRunScreen::updateObjects() 
{
	sb.run();
	Mover::stg = &stg;
	$P.mutex_tasklist.lock();
	Object::tl = &tl;
	std::vector<std::pair<phsc::Physics, std::string>> obj_info; // for debug
	for (auto t = tl.begin(); t != tl.end();)
	{
		gfw::Object* obj = getObject(t);
		Mover* mvr = static_cast<Mover*>(obj);
		mvr->run(data);
		obj_info.push_back(std::pair<phsc::Physics, std::string>(obj->Phys(), obj->Name()));
		if (obj->alive)
			t++;
		else
		{
			delete (Object*)t->workspace;
			t = tl.remove(t);
		}
	}
	objmng.run();
	$P.mutex_tasklist.unlock();
}

gfw::Screen::eScreenState JumpnRunScreen::run(gfw::BaseMedia& media)
{
	using namespace gmtr;
	auto e = eControl::RunJNR;

	
	if (player.Dst(tl))
	{
		Player* p = (Player*)player.Dst(tl);
		if ($P.turn_me >= 0)
		{
			p->Turned($P.turn_me);
			$P.turn_me = -1;
		}
	}

	if (counter == 160)
		$P.start_bgm = true;

	if (!$P.flags["prolog"])
	{
		data.players[0].vc->force_to_release = true;
		$P.bgm_name = "";
		if (counter == 60*8)
		{
			$P.flags["prolog"] = 1;
			Player* p = (Player*)player.Dst(tl);
			//p->Visible(true);
			data.players[0].vc->force_to_release = false;

			auto advscr = new adv::AdventureScreen(game);
			advscr->initialize(adv::AdventureScreen::texts[adv::AdventureScreen::texts.size() - 3]);
			controls.push_back(new OtherScreen($P.media, advscr));
			data.next_stage_name = "classroom_start.txt";
			data.next_start_p = $V(64 * 4, 384);
			controls.push_back(new CHANGE_STAGE());
		}
	}

	if (!$P.flags["prolog2"] && data.next_stage_name == "classroom1.txt")
	{
		data.players[0].vc->force_to_release = true;
		$P.bgm_name = "";
		//controls.push_back(new Prolog());
		if (counter == 60 * 5)
		{
			$P.flags["prolog2"] = 1;
			Player* p = (Player*)player.Dst(tl);
			//p->Visible(true);
			data.players[0].vc->force_to_release = false;

			auto advscr = new adv::AdventureScreen(game);
			advscr->initialize(adv::AdventureScreen::texts[adv::AdventureScreen::texts.size() - 2]);
			controls.push_back(new OtherScreen($P.media, advscr));
			data.next_stage_name = "classroom1.txt";
			data.next_start_p = $V(96, 384);
			controls.push_back(new CHANGE_STAGE());
		}
	}
	if (!$P.flags["lastbtl"]
		&& $P.flags["setcdl"]
		&& data.next_stage_name == "boss_room5.txt")
	{
		$P.flags["dropcdl"] = 0;
		$P.flags["lastbtl"] = 1;
		$P.flags["br5cls"] = 0;
		auto* ptr = generate<Item>(this->tl);
		ptr->initialize(REFPOINT_MAGICSTONE_X, REFPOINT_MAGICSTONE_Y + 64,
			$V(480 + 16, 416),
			-1, 0, $V(0, 0), 0, "dropcdl");
		ptr->LifeTime(99999);
		timer_lastbattle = 270;
	}
	//if ($P.flags["lastbtl"] && $P.flags["dropcdl"] && !$P.flags["lastrun"])
	if ($P.flags["dropcdl"] && !$P.flags["lastrun"])
	{
		//if (timer_lastbattle == 1 && player.Dst(tl) && !$P.flags["stg5c"])
		if (timer_lastbattle == 1 && player.Dst(tl))
		{
			//$P.flags["dspkyo"] = 1;
			data.encount_boss = 7;
			stg.BossPoint(672, 288);
		}
		else if (timer_lastbattle < 1)
			timer_lastbattle = 360;
	}

	if ($P.flags["kyo2ign"]) 
	{
		$P.flags["kyo2ign"] = false;
		stg.Curtain(255, 0, 0, 64);

		generate<EmitFire>(tl)->initialize(gmtr::Vector2D(64 * 4, 64 * 8), gmtr::Vector2D(0, -4)
			, 0, 20);
		generate<EmitFire>(tl)->initialize(gmtr::Vector2D(64 * 7, 64 * 8), gmtr::Vector2D(0, -4)
			, 0, 20);
		generate<EmitFire>(tl)->initialize(gmtr::Vector2D(64 * 11, 64 * 8), gmtr::Vector2D(0, -4)
			, 0, 20);
	}

	if ($P.flags["nbossb3"] && data.next_stage_name == "boss_room3.txt")
	{
		$P.appear_boss_dummy = true;
	}

	if ($P.flags["gameclear"])
	{
		controls.push_back(new Wait(240));
		controls.push_back(new END_JNR(eControl::ToEnding));
		$P.flags["gameclear"] = false;
		$P.flags["endwatched"] = true;
		//ラスボスと再戦できるようフラグ類を戻して億
		$P.flags["lastrun"] = 0;
		$P.flags["br5cls"] = 0;
		data.next_stage_name = "boss_room5.txt";
		data.next_start_p = $V(128, 448);
		$P.flags["nbossb6"] = 0;
		$P.flags["nbossb7"] = 0;
		//also reset mid boss flags to enable re-encounter
		$P.flags["nbossb2"] = 0;
		$P.flags["nbossb4"] = 0;
		$P.flags["nbossb9"] = 0;
		data.players[0].vc->force_to_release = true;
	}

	if (!controls.empty())
	{
		auto ctrl = controls.front();
		e = ctrl->run();
		if (!ctrl->Active())
		{
			delete ctrl;
			controls.pop_front();
		}
		switch (e)
		{
		default:
			break;
		case eControl::ToTitle:
			if (true)
			{
				Title* title = new Title(game);
				title->initialize();
				game->push_back(title);
			}
			return eScreenState::Finished;
			break;
		case eControl::ToStageSelect:
			if (true)
			{
				$P.media.musics.stopBGM();
				auto* title = new StageSelectionScreen(game);
				title->initialize();
				game->push_back(title);
			}
			return eScreenState::Finished;
			break;
		case eControl::ToEnding:
			if (true)
			{
				$P.flags["lastrun"] = false;
				$P.media.musics.stopBGM();
				Endroll* title = new Endroll(game);
				//Ending* title = new Ending(game);
				title->initialize();
				game->push_back(title);
				data.players[0].vc->force_to_release = false;
				data.next_stage_name = "fjtbroom.txt";
				data.next_start_p = 64 * $V(5, 3);
				data.pushToParmanent();
			}
			return eScreenState::Finished;
			break;
		case eControl::ToCardGallery:
			if (true)
			{
				spl::CardGalleryScreen* title = new spl::CardGalleryScreen(game);
				title->initialize();
				controls.push_back(new OtherScreen(media, title));
				controls.push_back(new PAUSE($P.players[0].vc));
			}
			return eScreenState::Continuing;
			break;
		case eControl::ToDeckEdit:
			if (true)
			{
				spl::DeckEditScreen* title = new spl::DeckEditScreen(game);
				title->initialize();
				controls.push_back(new OtherScreen(media, title));
				controls.push_back(new PAUSE($P.players[0].vc));
			}
			return eScreenState::Continuing;
			break;
		case eControl::ToConfig:
			if (true)
			{
				ConfigScreen* title = new ConfigScreen(game);
				title->initialize();
				controls.push_back(new OtherScreen(media, title));
				controls.push_back(new PAUSE($P.players[0].vc));
			}
			return eScreenState::Continuing;
			break;
		case eControl::ToAchievement:
			if (true)
			{
				AchievementScreen* title = new AchievementScreen(game);
				title->initialize();
				controls.push_back(new OtherScreen(media, title));
				controls.push_back(new PAUSE($P.players[0].vc));
			}
			return eScreenState::Continuing;
			break;
		case ChangeStage:
			if (true)
			{
				if (Character* p = (Character*)player.Dst(tl))
				{
					data.players[0].hp_current = p->HP();
					data.players[0].hp_max = p->HPMax();
					data.players[0].mp_current = p->MP();
					data.players[0].mp_max = p->MPMax();
					data.players[0].score = p->Score();
					data.turned = $P.turned = p->Turned();
				}
				data.pushToParmanent();
				$P.deck_cursor = ss.getCursor();
				JumpnRunScreen* title = new JumpnRunScreen(game);
				title->initialize();
				game->push_back(title);
				//title->controls.push_back(new CHANGE_STAGE());
			}
			return eScreenState::Finished;
			break;
		}
	}


	if (e == RunJNR)
	{
		data.pullDeckFromParmanent();

		Mover::stg = &stg;
		Mover::tl = &tl;
		if (timer_return_from_boss > 0)
			timer_return_from_boss--;
		if (timer_start_boss > 0)
			timer_start_boss--;
		if (timer_lastbattle > 0)
			timer_lastbattle--;

		for (auto& p : data.players)
		{
			p.deck.run();
			p.deck_tmp.run();
			p.deck.current_card = p.deck_tmp.current_card;
		}
		ss.run();
		bg.run(data);
		stg.run(data);

		if (player.Dst(tl))
		{
			Player* p = (Player*)player.Dst(tl);
			p->setXSpell(data.players[0].deck_tmp.current_card);
			if (sb.myhp > p->HP())
				sb.hp_shake_timer = 20;
			if (sb.mymp > p->MP())
				sb.mp_shake_timer = 20;
			sb.myhp = p->HP();
			sb.mymp = p->MP();
			sb.myhpmax = p->HPMax();
			sb.mympmax = p->MPMax();
			sb.myscore = p->Score();
			auto myp = p->P();
			data.players[0].hp_max = p->HPMax();
			data.players[0].mp_max = p->MPMax();
			$P.flags["turnport"] = (p->Center().x >= 64 * 12.5);

			if ($P.turn_me >= 0)
			{
				p->Turned($P.turn_me);
				$P.turn_me = -1;
			}

			if(!boss_encounted)
				data.camera.chase(myp.x - 320, myp.y - 200);
			objmng.setActiveWindow(data.camera.X(), data.camera.Y(), 640, 480);

			// カードがなくなったらデッキのリロードをする
			// もしかしたらこれはPlayerにやらせたほうが良いかも
			if (data.players[0].deck_tmp.finished() && data.players[0].deck_tmp.numCards() > 0)
			{
				if (data.players[0].timer_deckreload > 0)
				{
					data.players[0].timer_deckreload--;
					if (data.players[0].timer_deckreload == 0)
					{
						p->resetDeck();
						if(data.players[0].deck_tmp.numValidCards() > 0)
						p->MP() = p->MP() / 2;
					}
				}
				else
				{
					data.players[0].timer_deckreload = 300;
					//data.players[0].deck.reset();
					//p->resetDeck();
				}
			}
			if (p->alive == false)
			{
				player = Object::Pipe(nullptr, tl);
				data.players[0].max_score = max(data.players[0].max_score, data.players[0].score);
				data.players[0].score = 0;
				data.players[0].hp_current = data.players[0].hp_max;
			}
		}
		else
		{
			if (timer_restart < 0)
			{
				timer_restart = 210;
			}
			else
			{
				timer_restart--;
				if (timer_restart == 150)
				{
					$P.media.musics.resetBGM();
					$P.media.musics.startBGM("bgm\\gameover.mp3");

					$P.media.musics.setAutoRepeat(false);
				}
				if (timer_restart == 0)
				{
					$P.reset_bgm = true;
					data.camera.isFixed = false;
					timer_return_from_boss = -1;
					boss_beaten = false;
					boss_encounted = false;
					data.encount_boss = -1;

					$V myp = $V(320 + data.camera.X(), 240 + data.camera.Y());
					if (player.Dst(tl))
					{
						Player* p = (Player*)player.Dst(tl);
						myp = p->P();
					}

					controls.push_back(
						new CHANGE_STAGE(myp.x + 16 - data.camera.X(),
							myp.y + 16 - data.camera.Y()));
				}
			}
		}

		if (Player* p = (Player*)boss.Dst(tl))
		{
			sb.type = 1;
			sb.enemyhp = p->HP();
			sb.enemymp = p->MP();
			sb.enemyhpmax = p->HPMax();
			sb.enemympmax = p->MPMax();

			if (boss_encounted)
			{
				if (data.encount_boss == 2)
				{
					data.camera.chase(stg.BossPoint().x - 320 - 128, 192);
				}
				else if (data.encount_boss == 6)
				{
					data.camera.chase(stg.BossPoint().x - 640 + 96, 192 - 16);
				}
				else if (data.encount_boss == 7)
				{
					data.camera.chase(stg.BossPoint().x - 500, stg.BossPoint().y - 324 + 64 + 128);
					$P.flags["dspkyo"] = 1;
				}
				else if (data.encount_boss == 8)
				{
					data.camera.chase(stg.BossPoint().x - 320 - 128, stg.BossPoint().y - 200 + 48);
				}
				else if (data.encount_boss == 9)
				{
					data.camera.chase(stg.BossPoint().x - 320 - 128, stg.BossPoint().y - 200 + 48);
				}
				else
					data.camera.chase(stg.BossPoint().x - 320, stg.BossPoint().y - 200);
			}
			if (!p->alive)
			{
				sb.type = 0;
				timer_return_from_boss = 330;
				boss = Object::Pipe(nullptr, tl);
				boss_beaten = true;
				
			
				if (data.encount_boss == 2
					|| data.encount_boss == 4
					|| data.encount_boss == 9) //mid bosses
				{
					timer_return_from_boss = 60;
				}
				
			}
		}

		if (timer_return_from_boss == 240)
		{
			$P.media.musics.resetBGM();
			$P.media.musics.startBGM("bgm\\win.mp3");
			$P.media.musics.setAutoRepeat(false);
		}

		if (timer_return_from_boss == 110)
		{
			if (player.Dst(tl))
			{
				Player* p = (Player*)player.Dst(tl);
				p->guts(180);
			}
		}
		if (timer_return_from_boss == 0)
		{
			data.camera.isFixed = false;
			timer_return_from_boss = -1;
			boss_beaten = false;
			boss_encounted = false;

			bool player_alive = false;
			if (player.Dst(tl))
			{
				Player* p = (Player*)player.Dst(tl);
				player_alive = p->alive;
			}

			if (player_alive)
			{
				$P.flags["nbossb" + myutil::int2str(data.encount_boss)] =
					$P.getFlag("nbossb" + myutil::int2str(data.encount_boss)) + 1;

				//if not mid boss, start adv
				if (data.encount_boss != 2 && data.encount_boss != 4 && data.encount_boss != 9)
				{
					auto advscr = new adv::AdventureScreen(game);
					//advscr->initialize(adv::AdventureSamples::samples[data.encount_boss]);
					//advscr->initialize(adv::AdventureScreen::texts[data.encount_boss * 2 + 1]);
					std::string flag_name = "nbossb";
					int flag = $P.getFlag(flag_name + myutil::int2str(data.encount_boss));
					if (flag == 1)
						advscr->initialize(adv::AdventureScreen::texts[1 + data.encount_boss * 2]);
					else
						advscr->initialize(adv::AdventureScreen::texts_q[1 + data.encount_boss * 2]);
					controls.push_back(new OtherScreen($P.media, advscr));

					$P.media.musics.resetBGM();
					$P.media.musics.setAutoRepeat(true);
					if ($P.bgm_name.size() > 0)
						$P.media.musics.startBGM
						(std::string("bgm\\") + $P.bgm_name, $P.bgm_segno);
					else $P.media.musics.stopBGM();
				}
				if (data.encount_boss == 6)
				{
					$P.flags["setcdl"] = 1;
				}
				if (data.encount_boss == 7)
				{
					$P.flags["lastrun"] = 1;
				}

				$P.flags["nbossb" + myutil::int2str(data.encount_boss)] =
					$P.getFlag("nbossb" + myutil::int2str(data.encount_boss)) + 1;
				if (data.encount_boss == 0)
				{
					$P.flags["stg1c"] = 1;
					data.num_boss1_beaten++;
				}
				if (data.encount_boss == 1)
				{
					$P.flags["stg2c"] = 1;
					data.num_boss2_beaten++;
				}
				if (data.encount_boss == 2) //bee
				{
					//$P.flags["stg3c"] = 1;
					data.num_boss3_beaten++;
				}
				if (data.encount_boss == 3)
				{
					$P.flags["stg3c"] = 1;
					data.num_boss4_beaten++;
				}
				if (data.encount_boss == 8)
				{
					$P.flags["stg32c"] = 1;
					data.num_boss32_beaten++;
				}
				if (data.encount_boss == 4)
				{
					$P.flags["mbdt"] = true;
				}
				if (data.encount_boss == 5)
				{
					$P.flags["stg4c"] = 1;
					data.num_boss5_beaten++;
				}
				if (data.encount_boss == 7)//kyo2
				{
					$P.flags["stg5c"] = 1;
					$P.flags["pgbck"] = 1;
					data.num_boss7_beaten++;
				}
				if (data.encount_boss == 9) //bigfish
				{
					//$P.flags["stg3c"] = 1;
					data.num_boss9_beaten++;
				}
			
			}
			
			data.encount_boss = -1;
			sb.type = 0;
		}

		updateObjects();

		//buttons
		if (Constants::debugging)
		{
			if ($P.keycont.Z().pushed() && $P.keycont.X().isDown())
			{
				for (auto t = tl.begin(); t != tl.end(); t++)
				{
					gfw::Object* obj = getObject(t);
					if (obj->Name().substr(0, 9) == "character" && obj != player.Dst(tl))
						static_cast<Character*>(obj)->HP() = 0;
				}
			}
			if ($P.keycont.A().pushed() && $P.keycont.S().isDown())
			{
				if (player.Dst(tl))
				{
					Player* p = (Player*)player.Dst(tl);
					p->HP() = 0;
				}
			}
		}

		if (data.players[0].vc->Space().pushed())
		{
			paused = true;
			controls.push_back(new PAUSE(data.players[0].vc));
		}
		else if (data.change_stage >= 0)
		{
			//controls.push_back(new CHANGE_STAGE());
			$V myp = $V(320  + data.camera.X(), 240 + data.camera.Y());
			if (player.Dst(tl))
			{
				Player* p = (Player*)player.Dst(tl);
				myp = p->P();
			}

			controls.push_back(
				new CHANGE_STAGE(myp.x + 16 - data.camera.X(),
					myp.y + 16 - data.camera.Y()));
			data.change_stage = -1;
		}
		else if (data.encount_boss >= 0
			&& !boss_encounted && timer_start_boss == -1)
		{
			timer_start_boss = 60;
			data.players[0].vc->force_to_release = true;
		}
		else if (timer_start_boss == 2)
		{
			//mid bosses, skip advs
			if (data.encount_boss == 2 || data.encount_boss == 4 || data.encount_boss == 9)
			{
				std::string flag_name = "nbossb";
				int flag = $P.getFlag(flag_name + myutil::int2str(data.encount_boss));
				$P.skip_boss = flag != 0;
				//if(!flag)
				data.players[0].vc->force_to_release = false;
			}
			else
			{
				$P.skip_boss = false;
				data.players[0].vc->force_to_release = false;
				auto advscr = new adv::AdventureScreen(game);
				std::string flag_name = "nbossb";
				int flag = $P.getFlag(flag_name + myutil::int2str(data.encount_boss));
				//epilog
				if (data.encount_boss == 10) {
					advscr->initialize(
						adv::AdventureScreen::texts[adv::AdventureScreen::texts.size() - 1]
					);
					$P.skip_boss = true;

				}
				else if(flag == 0)
					advscr->initialize(adv::AdventureScreen::texts[data.encount_boss * 2]);
				else
					advscr->initialize(adv::AdventureScreen::texts_q[data.encount_boss * 2]);
				
				controls.push_back(new OtherScreen($P.media, advscr));
			}
		}
		else if (timer_start_boss == 1)
		{
			if (!$P.skip_boss)
			{
				$P.skip_boss = false;
				timer_start_boss = -1;
				auto* p = BossFactory().create(tl, data, stg.BossPoint());
				boss = Object::Pipe(p, tl);
				//data.camera.setPos(stg.BossPoint().x - 320, stg.BossPoint().y - 240 - 64);
				data.camera.isFixed = true;
				data.in_boss_battle = true;
				boss_encounted = true;

				if (data.encount_boss == 7) 
				{
					// moved to adv screen
					//$P.media.musics.resetBGM();
					//$P.media.musics.startBGM("bgm\\lastbattle.mp3", 0.0);
				}
				else if (data.encount_boss != 2 && data.encount_boss != 4 
					&& data.encount_boss != 7 && data.encount_boss != 9)
				{
					$P.media.musics.resetBGM();
					$P.media.musics.startBGM("bgm\\battle.mp3", 3.3);
				}
				sb.type = 1;
			}
			else
			{
				data.encount_boss = -1;
				data.camera.isFixed = false;
				timer_return_from_boss = -1;
				boss_beaten = false;
				boss_encounted = false;
			}
		}

		if (data.open_gacha)
		{
			spl::GachaponScreen* scr = new spl::GachaponScreen(this->game);
			scr->initialize();
			controls.push_back(new OtherScreen(media, scr));
			data.open_gacha = false;
		}

		if (data.message_time >= 0)
		{
			//sb.type = 2;
			sb.message = data.message;
			sb.message_timer = data.message_time;
			data.message_time = -1;
		}
		counter++;
	}
	return eScreenState::Continuing;
}

void JumpnRunScreen::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	//fps.run();
	//graphics->draw(Parmanent::Instance().img_map, 0, 0, 0, 0, 0, 960, 320);
	bg.render(media, data);
	stg.render(media, data);
	/*if (player.Dst(tl))
	{
		Player* p = (Player*)player.Dst(tl);
		auto v = p->P() - $V(0, 32);
		graphics->draw($P.img_obj, v.x, v.y, 0, REFPOINT_MY_MARKER_X, REFPOINT_MY_MARKER_Y, 32, 32);
	}*/
	$P.mutex_tasklist.lock();
	Object::tl = &tl;
	for (auto t = tl.begin(); t != tl.end(); t++)
	{
		Object* obj = getObject(t);
		//std::cout << obj->Name() << std::endl;
		((Mover*)obj)->render(media, data);
	}
	$P.mutex_tasklist.unlock();
	stg.renderForeground(media, data);

	ss.render(media);
	sb.render(media);

	if (!controls.empty())
	{
		controls.front()->render(media);
		//graphics->clear(0, 0, 0, 64);
	}
	if (Constants::debugging)
	{
		graphics->drawString("debugging", 0, 0, 72, 20, 1, 0, 0, 1);
	}

}