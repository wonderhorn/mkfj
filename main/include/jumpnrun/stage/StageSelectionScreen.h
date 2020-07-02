#include"framework/Screen.h"
#include"framework/Control.h"
#include<future>
//#include <boost/thread/future.hpp>

namespace jnr
{
	class JumpnRunScreen;
}
;

namespace gfw
{
	enum ePanelStatus{ Cleared, Uncleared, Invisible };
	class StagePoint
	{
	public:
		int x, y;
		std::string name;
		std::string stg_file_name;
		int start_x, start_y;
		std::vector<int> neighbors;
		std::vector<std::string> stage_prefixes;
		ePanelStatus status;
	};
	class StageSelectionScreen : public gfw::Screen
	{
	public:
		StageSelectionScreen(gfw::Game* game) : gfw::Screen(game){}
		bool initialize();
		void finalize();
		eScreenState run(gfw::BaseMedia& media);
		void render(gfw::BaseMedia& media);

	private:
		int cursor;
		int age;
		std::vector<StagePoint> stage_point;
		int p_current, p_prev;
		int timer_move, timer_end, timer_wink;
		std::deque<jnr::CONTROL *> controls;
		//std::future<jnr::JumpnRunScreen *> future_jnrscr;
		jnr::JumpnRunScreen * jnrscr;
	};
};