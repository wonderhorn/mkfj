#pragma once
#include"framework/BaseMedia.h"
#include"framework/VirtualController.h"
namespace gfw{ class Screen; };
namespace jnr
{
	enum eControl{Continue, ToDeckEdit, ToCardGallery, ToTitle, ToEnding
		, ToConfig, ToAchievement, ChangeStage, RunJNR, ToStageSelect};
	class CONTROL
	{
	public:
		CONTROL();
		virtual eControl run() = 0;
		virtual void render(gfw::BaseMedia& media) = 0;
		bool Active(){ return isActive; }

	protected:
		bool isActive;
	};

	class PAUSE : public CONTROL
	{
	public:
		PAUSE(gfw::VirtualController* vc);
		eControl run();
		void render(gfw::BaseMedia& media);
		int Cursor(){ return cursory; }

	private:
		int age, timer_exit;
		int cursory;
		gfw::VirtualController* vc;
		bool for_stageselect;
		eControl selected;
	};

	class Wait: public CONTROL
	{
	public:
		Wait(int t);
		eControl run();
		void render(gfw::BaseMedia& media){}

	private:
		int age, timer;
	};

	class CHANGE_STAGE : public CONTROL
	{
	public:
		CHANGE_STAGE();
		CHANGE_STAGE(int x, int y);
		eControl run();
		void render(gfw::BaseMedia& media);

	private:
		int age;
		int x, y;
	};

	class END_JNR : public CONTROL
	{
	public:
		END_JNR(eControl e);
		eControl run();
		void render(gfw::BaseMedia& media);

	private:
		eControl e;
		int age;
	};

	class START_STAGE : public CONTROL
	{
	public:
		START_STAGE();
		START_STAGE(int x, int y);
		eControl run();
		void render(gfw::BaseMedia& media);

	private:
		int age;
		int x, y;
	};

	class OtherScreen : public CONTROL
	{
	public:
		OtherScreen(gfw::BaseMedia& media, gfw::Screen* scr);
		eControl run();
		void render(gfw::BaseMedia& media);

	private:
		int age;
		gfw::Screen* scr;
		gfw::BaseMedia& media;
	};

	class Prolog : public CONTROL
	{
	public:
		Prolog();
		eControl run();
		void render(gfw::BaseMedia& media);

	private:
		int age;
	};
};