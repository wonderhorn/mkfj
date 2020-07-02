#include<iostream>
//#include<filesystem> //C++17
#include<fstream>
#include<sstream>
#include <sys/stat.h>
#include <sys/types.h>

#include<jumpnrun/system/Parmanent.h>
#include<framework/Game.h>
#include<utils/MyUtil.h>
#include<utils/string_util.h>

#include <SDL2/SDL.h>

bool initialized = false;
SDL_Window* sdl_window = nullptr;
SDL_Renderer* sdl_renderer = nullptr;

void initParmanentsForReplay(const std::string& str_t)
{
	$P.reset();
	$P.players.push_back(jnr::PlayerData(0, std::string("player1"), &$P.replaycont, jnr::Deck(), true));
	$P.players.push_back(jnr::PlayerData(0, std::string("boss1"), &$P.ai.Controller(), jnr::Deck(), true));

	unsigned int t = time(nullptr);
	//std::string str_t = myutil::int2str(t);
	//std::string str_t = "1463300779";
	$P.start_time = str_t;
	$P.save_file_name = (std::string("replay/") + str_t + "/savedata.txt").c_str();
	$P.load($P.save_file_name);
	$P.replaycont.readFile("replay/" + str_t + "/controller.txt");

	$P.mutex_rand.lock();
	$P.rand.reset(myutil::str2int($P.start_time));
	$P.mutex_rand.unlock();
	$P.to_save = false;
}
void initParmanentsForPlay()
{
	$P.reset();
	$P.players.push_back(jnr::PlayerData(0, std::string("player1"), &$P.andcont, jnr::Deck(), true));
	$P.players.push_back(jnr::PlayerData(0, std::string("boss1"), &$P.ai.Controller(), jnr::Deck(), true));
	std::ifstream ifs("savedata/latest.txt");
	if(! ifs.is_open())
	{
		$P.reset();
		$P.window_w = 640;
		$P.window_h = 480;
	}
	$P.load();
	unsigned int t = time(nullptr);
	std::string str_t = myutil::int2str(t);
	$P.start_time = str_t;
	mkdir((std::string("replay/") + str_t).c_str(), 0755);
	$P.save((std::string("replay/") + str_t + "/" + "savedata.txt").c_str());

	$P.mutex_rand.lock();
	$P.rand.reset(myutil::str2int($P.start_time));
	$P.mutex_rand.unlock();
	$P.to_save = true;
}

void sdl_init()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  sdl_window = SDL_CreateWindow("test",
    0, 0, 640, 480, SDL_WINDOW_SHOWN);
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

int main()
//INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR cmd, INT)
{
	resetLog();
	bool rendering = true;
	//OutputDebugString("program started\n");
	//char cdir[1000];
	//GetCurrentDirectory(1000, cdir);
	//OutputDebugString((std::string("") + cdir + "\n").c_str());

	//initParmanentsForReplay("1486482314");
	initParmanentsForPlay();

	//hWnd = InitWindow(TRUE, "mkfj v030", 640, 480);
	//hWnd = InitWindow(TRUE, "mkfj v010", 960, 720);
	//hWnd = InitWindow(TRUE, "mkfj v010", 320, 240);
	//ShowWindow(hWnd, SW_SHOWDEFAULT);
	//UpdateWindow(hWnd);
  sdl_init();

	gfw::Game game;
	gfw::MediaToFile& media = jnr::Parmanent::Instance().media;
	gfw::MediaToFile& media_to_file = jnr::Parmanent::Instance().media_to_file;
	bool b = false;
	$P.current_window_w = 640;
	$P.window_w = 640;
	$P.current_window_h = 480;
	$P.window_h = 480;
	//media.initialize(hWnd, wc, $P.window_w, $P.window_h);
	media.initialize($P.window_w, $P.window_h, "tmp.txt");
	media_to_file.initialize($P.window_w, $P.window_h,
		std::string("replay/") + $P.start_time + "/" + "drawings.txt");
	$P.loadAllResourses();
	media.sounds.Volume($P.volume_se);
	media.musics.setVolume($P.volume_bgm);
	game.initialize();
	//graphics->resizeWindow(1280, 480);
	//graphics->resizeWindow(1304, 480);

	initialized = true;
	bool in_loop = true;
  int counter = 0;
	while (in_loop)
	{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
          in_loop = false;
          break;
        }
    }

    SDL_RenderClear(sdl_renderer);
    SDL_RenderPresent(sdl_renderer);

    std::cout << "#-- frame " << counter << " --" << std::endl;

		if ($P.reset_bgm && $P.start_bgm)
		{
			$P.media.musics.resetBGM();
			$P.media.musics.setAutoRepeat(true);
			if ($P.bgm_name.size() > 0)
				$P.media.musics.startBGM
					(std::string("bgm/") + $P.bgm_name, $P.bgm_segno);
			else media.musics.stopBGM();
			$P.reset_bgm = false;
			$P.start_bgm = false;
		}

		media.input.poll();

		gfw::Game::eGameState gamestate = game.run(media);
		if (rendering & media.graphics.startRendering())
		{
			game.render(media);

			/*if (media_to_file.graphics.startRendering())
			{
				game.render(media_to_file);
				media_to_file.graphics.endRendering();
			}*/

			if (!in_loop)
			{
				if ($P.to_save)
				{
					media.graphics.screenshot(std::string("replay/") + $P.start_time + "/ss.png");
				}
			}

			media.graphics.endRendering();
		}

		if (gamestate == gfw::Game::Finished)
		{
			/*if ($P.to_save)
			{
				graphics->screenshot(std::string("replay/") + $P.start_time + "\\ss.bmp");
			}*/
			game.finalize();
			break;
		}
    counter++;
	};
	media_to_file.finalize();
	$P.aisuperviser.save(std::string("AISupervision/") + $P.start_time + ".csv");

	media.graphics.releaseImage($P.img_sky);
	media.graphics.releaseImage($P.img_obj);
	media.graphics.releaseImage($P.img_map);
	media.graphics.releaseImage($P.img_statusboard);
	media.graphics.releaseImage($P.img_menu);
	media.finalize();

	/*$P.iothread.end();
	std::stringstream ss;
	std::cin.rdbuf(ss.rdbuf());
	ss << "finished" << std::endl;*/
	return 0;
}
