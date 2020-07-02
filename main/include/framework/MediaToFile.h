#pragma once

#include<vector>
#include<string>

#include"framework/BaseMedia.h"

#define MediaAccessor Media::instance
#define GraphicsAccessor Media::instance.graphics


namespace gfw
{
	class GraphicsToFileRenderedObject
	{
	public:
		double x, y, w, h, theta, magx, magy;
		int refx, refy;
		int blend;
		bool turned;
		int image;
	};

	class GraphicsToFile: public BaseGraphics
	{
	public:
		GraphicsToFile();
		~GraphicsToFile();
		bool initialize(const std::string& filename);
		void finalize();
		ImageHandler loadImage(const std::string& file_name, int w, int h, int color_key = 0);
		ImageHandler createRenderingTarget(int w, int h);
		void releaseImage(ImageHandler ih);

		bool draw(ImageHandler ih, int x, int y, int z, int refx, int refy, int w, int h, bool turn = false, int blend = 0xffffffff);
		bool drawSpinResize(ImageHandler ih, int x, int y, int z, int refx, int refy, int w, int h, bool turn, int blend, float magx, float magy, float theta);
		bool drawSpinResizeNoAlpha(ImageHandler ih, int x, int y, int z, int refx, int refy, int w, int h, bool turn, int blend, float magx, float magy, float theta);
		bool drawLine(int orix, int oriy, int terx, int tery, int r, int g, int b, int a, int width);
		bool drawString(const std::string& text, int x, int y, int width, int height
			, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
		bool clear(int r, int g, int b, int a);
		bool Whipe(int x, int y, int r) { return false; }
		bool startRendering();
		void endRendering();
		bool setRenderingTarget(ImageHandler img);
		bool setRenderingTargetToBackBuffer();
		void resizeWindow(int w, int h);
		void releaseAllImages();
		//void screenshot(const std::string& file_name);

		bool windowed;
		int window_width, window_height;

	private:
		//std::vector<LPDIRECT3DTEXTURE9> images;
		std::string filename;
		std::vector<std::string> images;
		std::vector<std::vector<GraphicsToFileRenderedObject>> frames;
		int current_frame_idx;
	};

	class SoundsToFile : public BaseSounds
	{
	public:
		SoundsToFile() {}
		~SoundsToFile() {}
		bool initialize(){};
		void finalize() {}
		void play(SoundHandler snd) {}
		void Volume(int v) { }
		SoundHandler loadSound(const std::string& filename, double base_volume = 1.0) { return 0; }
	private:
	};

	class MediaToFile: public BaseMedia
	{
	public:
		MediaToFile() {}
		bool initialize(int window_width, int window_height, const std::string& file_name);
		void finalize();

		GraphicsToFile graphics;
		SoundsToFile sounds;

		virtual BaseGraphics* getGraphics() { return &graphics; }
		virtual BaseSounds* getSounds() { return &sounds; }

		static MediaToFile instance;
	};
}