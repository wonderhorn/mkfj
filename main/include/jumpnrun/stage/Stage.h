#pragma once
#include"framework/MediaAlternative.h"
#include"utils/Geometry.h"
#include"jumpnrun/stage/StageDataLoader.h"
#include"jumpnrun/system/JumpnRunData.h"
#include"jumpnrun/mapchip.h"
namespace jnr
{
	class Mapchip
	{
	public:
		Mapchip() : x(-1), y(-1), refx(-1), refy(-1), w(-1), h(-1), type('n')
			, foreground(false), waterproof(false), alpha(0xffffffff){}
		int x, y, w, h, refx, refy;
		char type;
		bool foreground, waterproof;
		int alpha;
		std::string data;
		const static int WIDTH = 64, HEIGHT = 64;
		static LIST_OF_MAPCHIP Ref2Chip(int refx, int refy);
	};

	class Billboard
	{
	public:
		int x, y, refx, refy, w, h;
		std::string file_name;
		double translate_rate_x, translate_rate_y;
	};

	class Stage
	{
		friend class StageDataLoader;
	public:
		Stage();
		~Stage();
		bool initialize();
		void finalize();
		void run(jnr::JumpnRunData& data);
		void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		void renderForeground(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		const Mapchip& Chip(int index_x, int index_y)const;
		Mapchip& ChipMutable(int index_x, int index_y);
		const Mapchip& ChipAt(const gmtr::Vector2D& v)const;
		void initializeTestStage();
		bool enterable(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next) const;
		bool enterable(const gmtr::Vector2D& v) const;
		gmtr::Vector2D react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up);
		char getType(const gmtr::Vector2D& v)const;
		void generateClouds();
		void generateRainDrops();
		void generateBirds();
		int Width()const{ return width * Mapchip::WIDTH; }
		int Height()const{ return height * Mapchip::HEIGHT; }
		gmtr::Vector2D StartPoint()const{ return startp; }
		void StartPoint(const gmtr::Vector2D& p){ startp = p; }
		gmtr::Vector2D BossPoint()const { return bossp; }
		void BossPoint(int x, int y) { bossp.x = x; bossp.y = y; }
		void Curtain(int r, int g, int b, int a);
		static Stage readFile(const std::string& filename);

	private:
		int width, height;
		int age;
		bool rainy;
		int curtainr, curtaing, curtainb, curtaina;
		gmtr::Vector2D startp, bossp;
		Mapchip* chips;
		Mapchip dummy;
		std::vector<Billboard> billboards;
		std::vector<Billboard> clouds;
		std::vector<Billboard> raindrops;
		std::vector<Billboard> birds;
	};
};