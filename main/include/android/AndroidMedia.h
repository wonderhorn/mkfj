#pragma once
#include"framework/BaseMedia.h"
#include"android/GLGraphic.h"
#include<vector>
#include<string>

namespace gfw
{

	class Graphics : public BaseGraphics
	{
	public:
		Graphics();
		~Graphics();
		bool initialize(GLGraphic* glg);
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
		bool wipe(int x, int y, int r);
		bool clear(int r, int g, int b, int a);
		bool startRendering();
		void endRendering();
		bool setRenderingTarget(ImageHandler img);
		bool setRenderingTargetToBackBuffer();
		void resizeWindow(int w, int h);
		void releaseAllImages();
		void screenshot(const std::string& file_name);

		bool windowed;
		int window_width, window_height;

	private:
		class ImageData {
		public:
			GLuint texh;
			int w, h;
			int colorkey;
		};
		std::vector<ImageData> images;
		GLGraphic* glg;

	};

	class Sounds : public BaseSounds
	{
	public:
		Sounds();
		~Sounds();
		bool initialize();
		void finalize();
		void play(SoundHandler snd);
		void Volume(int v) { this->volume = v; }
		SoundHandler loadSound(const std::string& filename, double base_volume = 1.0);
	private:
		int num_sound;
		int volume;
		std::vector<double> volumes;
	};

	class Musics
	{
	public:
		Musics();
		bool initialize();
		bool startBGM(const std::string& name, double intro_time = 0.0);
		void stopBGM();
		void resetBGM();
		void finalize();
		void checkLoop(long evCode, long param1, long param2);
		void setVolume(int v);
		void setAutoRepeat(bool b);

	};

	class Input : public BaseInput
	{
	public:
		Input();
		bool initialize();
		void finalize();
		bool poll();
		int deviceNum()const { return 1; }

		/*class Pad
		{
		public:
			Button button[32], up, right, down, left;
		}pads[256];*/

		void touch(int x, int y);
		void setZRect(int z_x, int z_y, int  z_w, int z_h);
		void setXRect(int z_x, int z_y, int  z_w, int z_h);
		void setUpRect(int z_x, int z_y, int  z_w, int z_h);
		void setRightRect(int z_x, int z_y, int  z_w, int z_h);
		void setDownRect(int z_x, int z_y, int  z_w, int z_h);
		void setLeftRect(int z_x, int z_y,int  z_w, int z_h);

		//Button key_z, key_x, key_c, key_a, key_s, key_space, key_up, key_right, key_down, key_left;
		int z_x, z_y, z_w, z_h;
		int x_x, x_y, x_w, x_h;
		int up_x, up_y, up_w, up_h;
		int right_x, right_y, right_w, right_h;
		int down_x, down_y, down_w, down_h;
		int left_x,left_y, left_w, left_h;
		int last_touch_x, last_touch_y;

	private:
	};

	class Media : public BaseMedia
	{
	public:
		Media();
		bool initialize(GLGraphic* glg);
		void finalize();

		Graphics graphics;
		Sounds sounds;
		Musics musics;
		Input input;

		virtual BaseGraphics* getGraphics() { return &graphics; }
		virtual BaseSounds* getSounds() { return &sounds; }

		static Media instance;

	private:
	};
}