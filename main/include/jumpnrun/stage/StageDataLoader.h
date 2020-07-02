#pragma once
#include<string>

namespace gfw
{
	class ObjectManager;
	class Tasklist;
};

namespace jnr
{
	class Stage;
	class Background;
	class StageDataLoader
	{
	public:
		bool load(const std::string file_name, gfw::Tasklist* tl, Stage& stg
			, gfw::ObjectManager& om, Background* bg = nullptr);
		void createPlainStage(int width, int height, gfw::Tasklist* tl, Stage& stg
			, gfw::ObjectManager& om, Background* bg = nullptr);
	};
};