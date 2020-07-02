#pragma once
#include"framework/MediaAlternative.h"
#include<array>

typedef std::array<bool, 10> ButtonData;

namespace gfw
{
	class VirtualController
	{
	public:
		VirtualController() : force_to_release(false){}
		virtual bool poll() = 0;
		const BaseInput::Button& Z()const{ return force_to_release ? allways_off: z; }
		const BaseInput::Button& X()const{ return force_to_release ? allways_off : x; }
		const BaseInput::Button& C()const{ return force_to_release ? allways_off : c; }
		const BaseInput::Button& A()const{ return force_to_release ? allways_off : a; }
		const BaseInput::Button& S()const{ return force_to_release ? allways_off : s; }
		const BaseInput::Button& Space()const{ return force_to_release ? allways_off : space; }
		const BaseInput::Button& Up()const{ return force_to_release ? allways_off : up; }
		const BaseInput::Button& Right()const{ return force_to_release ? allways_off : right; }
		const BaseInput::Button& Down()const{ return force_to_release ? allways_off : down; }
		const BaseInput::Button& Left()const{ return force_to_release ? allways_off : left; }
		bool force_to_release;
	protected:
		BaseInput::Button z, x, c, a, s, space, up, right, down, left, allways_off;
	};

	class KeyboardController : public VirtualController
	{
	public:
		KeyboardController(BaseInput& input);
		bool poll();
	private:
		BaseInput& input;
	};

	class JoyPadController: public VirtualController
	{
	public:
		JoyPadController(BaseInput& input);
		bool poll();
		int getPushedButtonId();
		int btnidx_a, btnidx_s, btnidx_z, btnidx_x, btnidx_c, btnidx_space;
	private:
		BaseInput& input;
		int pad_id;
	};

	class ManualController : public gfw::VirtualController
	{
	public:
		ManualController() : push_z(false), push_x(false), push_c(false), push_a(false), push_s(false)
			, push_up(false), push_right(false), push_down(false), push_left(false){}
		bool poll()
		{
			z.setValue(push_z); x.setValue(push_x); c.setValue(push_c);
			a.setValue(push_a); s.setValue(push_s); up.setValue(push_up); right.setValue(push_right); down.setValue(push_down); left.setValue(push_left);
			push_z = push_x = push_c = push_a = push_s = push_up = push_right = push_down = push_left = false;
			return true;
		}
		bool push_z, push_x, push_c, push_a, push_s, push_up, push_right, push_down, push_left;
	};

	class ReplayController : public gfw::VirtualController
	{
	public:
		ReplayController();
		bool readFile(const std::string& file_name);
		bool poll();
		bool push_z, push_x, push_c, push_a, push_s, push_up
			, push_right, push_down, push_left, push_space;

		int cursor;
		std::vector<ButtonData> data;

		static std::vector<ButtonData> test_data();
	};

	class AndController : public VirtualController
	{
	public:
		AndController(VirtualController* vc1, VirtualController* vc2);
		bool poll();
		void VC1(VirtualController* vc1){ this->vc1 = vc1; }
		void VC2(VirtualController* vc2){ this->vc2 = vc2; }
	private:
		VirtualController* vc1, *vc2;
	};

	class ControllerRecorder
	{
	public:
		void clear(){ data.clear(); }
		void push(const ButtonData& buttons);
		bool writeFile(const std::string& file_name)const;
		bool readFile(const std::string& file_name);
		std::vector<ButtonData> data;
		std::vector<std::string> comments;
	};
};