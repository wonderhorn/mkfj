#pragma once
#include<string>

typedef int ImageHandler;
typedef int SoundHandler;
typedef int MusiCANDLEr;

namespace gfw
{
	class BaseGraphics
	{
	public:
		virtual bool draw(ImageHandler ih, int x, int y, int z,
			int refx, int refy, int w, int h, bool turn = false, int blend = 0xffffffff) = 0;
		virtual bool drawSpinResize(ImageHandler ih, int x, int y, int z, int refx, int refy,
			int w, int h, bool turn, int blend, float magx, float magy, float theta) = 0;
		virtual bool drawSpinResizeNoAlpha(ImageHandler ih, int x, int y, int z, int refx, int refy,
			int w, int h, bool turn, int blend, float magx, float magy, float theta) = 0;
		virtual bool drawLine(int orix, int oriy, int terx, int tery,
			int r, int g, int b, int a, int width) = 0;
		virtual bool drawString(const std::string& text, int x, int y, int width, int height
			, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) = 0;
		virtual bool clear(int r, int g, int b, int a) = 0;
		virtual bool Whipe(int x, int y, int r) = 0;
		virtual bool startRendering() = 0;
		virtual void endRendering() = 0;
		virtual bool setRenderingTarget(ImageHandler img) = 0;
		virtual bool setRenderingTargetToBackBuffer() = 0;
	};

	class BaseSounds
	{
	public:
		virtual void play(SoundHandler snd) = 0;
	};

	class BaseMedia
	{
	public:
		virtual BaseGraphics* getGraphics() { return nullptr; }
		virtual BaseSounds* getSounds() { return nullptr; }
	};

	class BaseInput
	{
	public:
		BaseInput(){}
		virtual bool initialize(){ return true; }
		virtual void finalize() {  }
		virtual bool poll() { return false; }
		virtual void deviceChange() {}

		class Button
		{
		public:
			Button(const std::string& name = "") {}
			bool isUp() const { return !value; }
			bool isDown()const { return value; }
			bool pushed()const { return value && !value_prev; }
			bool pulled()const { return !value && value_prev; }
			void setValue(bool b) { value_prev = value; value = b; }
		private:
			int id;
			bool value, value_prev;
			//std::string name;
		};
		Button key_z, key_x, key_c, key_a, key_s, key_space, key_up, key_right, key_down, key_left;

		class Pad
		{
		public:
			Button button[32], up, right, down, left;
		}pads[256];
	};

}