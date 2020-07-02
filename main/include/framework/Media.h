#pragma once

#include<vector>
#include<string>

#include"framework/BaseMedia.h"
#include"directx/DXlib.h"
#include"directx/DXFONT.h"
#include"directx/DIController.h"
#include"directx/DSOUND.h"
#include"directx/DSHOW.h"

#define MediaAccessor Media::instance
#define GraphicsAccessor Media::instance.graphics

namespace gfw
{

	class Graphics: public BaseGraphics
	{
	public:
		Graphics();
		~Graphics();
		bool initialize(HWND hWnd);
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
		bool Whipe(int x, int y, int r);
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
		HWND hwnd;
		std::vector<LPDIRECT3DTEXTURE9> images;
		CD3DSetup* d3d;
		LPDIRECT3DSURFACE9 pBackBuf;
	};

	class Sounds: public BaseSounds
	{
	public:
		Sounds();
		~Sounds();
		bool initialize(HWND hWnd);
		void finalize();
		void play(SoundHandler snd);
		void Volume(int v){ this->volume = v; }
		SoundHandler loadSound(const std::string& filename, double base_volume = 1.0);
	private:
		DSOUND* dsound;
		int num_sound;
		int volume;
		std::vector<double> volumes;
	};

	class Musics
	{
	public:
		Musics();
		bool initialize(HWND hWnd, WNDCLASSEX wc);
		bool startBGM(const std::string& name, double intro_time = 0.0, double start_time = 0.0);
		void stopBGM();
		void resetBGM();
		void finalize();
		void checkLoop(long evCode, LONG param1, LONG param2);
		void setVolume(int v);
		void setAutoRepeat(bool b);

		BGM* bgm;
	};

	class Input : public BaseInput
	{
	public:
		Input();
		bool initialize(HWND hWnd, WNDCLASSEX wc);
		void finalize();
		bool poll ();
		int deviceNum()const;
		void deviceChange();

		/*class Button
		{
		public:
			Button(const std::string& name = "");
			bool isUp() const{ return !value; }
			bool isDown()const{ return value; }
			bool pushed()const{ return value && !value_prev; }
			bool pulled()const{ return !value && value_prev; }
			void setValue(bool b){ value_prev = value; value = b; }
		private:
			int id;
			bool value, value_prev;
			//std::string name;
		};*/

		//Button key_z, key_x, key_c, key_a, key_s, key_space, key_up, key_right, key_down, key_left;

	private:
		DINPUT* dinput;
	};

	class Media : public BaseMedia
	{
	public:
		Media();
		bool initialize(HWND hWnd, WNDCLASSEX wc, int window_width, int window_height);
		void finalize();

		Graphics graphics;
		Sounds sounds;
		Musics musics;
		Input input;

		virtual BaseGraphics* getGraphics() { return &graphics; }
		virtual BaseSounds* getSounds() { return &sounds; }

		static Media instance;
	};
}