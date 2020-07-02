#pragma once
#include"framework/MediaAlternative.h"
#include"jumpnrun/system/JumpnRunData.h"
#include"jumpnrun/stage/StageDataLoader.h"
#include<vector>

namespace jnr
{
	class BackgroundObject
	{
	public:
		BackgroundObject();
		BackgroundObject(int refx, int refy, int width, int height
			, int x, int y, double scale, ImageHandler img);
		int refx, refy, width, height, x, y;
		double scale;
		ImageHandler img;
	};
	class Background
	{
		friend class StageDataLoader;
	public:
		Background();
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, jnr::JumpnRunData& data);
		int offset_x, offset_y, img_width, img_height;
		int offset_x_min, offset_y_min, offset_x_max, offset_y_max;
		double translate_rate_x, translate_rate_y;

	private:
		std::vector<BackgroundObject> objects;
		ImageHandler img;
	};
};