#include"framework/Screen.h"
#include"framework/Task.h"
#include"jumpnrun/system/Widget.h"
#include"adv/Script.h"
#include"adv/CharacterImageSet.h"
#include<string>
#include<map>
#include<strstream>

namespace adv
{
	class AdventureScreen : public gfw::Screen
	{
	public:
		AdventureScreen(gfw::Game* game);
		~AdventureScreen();
		bool initialize();
		bool initialize(const std::string& str);
		void finalize();
		gfw::Screen::eScreenState run(gfw::BaseMedia& media);
		void render(gfw::BaseMedia& media);
		static std::vector<std::string> texts, texts_q;

	private:
		std::string script_name, text, speaker_name;
		gfw::Tasklist tl;
		jnr::StatusBoard sb;
		int counter, timer_wait, cursor_text;
		bool waiting_key, finished;
		std::map<std::string, gfw::Object::Pipe> name2chara;
		std::map<std::string, CharacterImageSet> name2images;
		std::map<std::string, SoundHandler> name2sound;
		std::map<std::string, int> vars;
		std::strstream ss;
		Script script;
		int speed_text;
		int ask_cursor;
		bool item_board_visible;
		int item_borad_refx, item_borad_refy;
		std::string ask_var_name;
	};

	class AdventureSamples
	{
	public:
		static std::vector<std::string> samples;
	};
};