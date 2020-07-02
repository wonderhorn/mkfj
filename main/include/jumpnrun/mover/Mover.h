#pragma once
#include"framework/Object.h"
#include"jumpnrun/system/JumpnRunScreen.h"
#include"utils/Geometry.h"
#include"jumpnrun/stage/Stage.h"
#include"utils/OwnerHandle.h"
#include"jumpnrun/system/JumpnRunData.h"
#include<map>
#include<string.h>
#define _USE_MATH_DEFINES
#include<math.h>
#define JNR_FLAG_SIZE 12

#ifndef GFW_WIN32
#define strncpy_s(a, b, c) strncpy((a), (b), (c))
#endif

#define generateMover(p, tl) (assert(MoverClassRegisterer::Entries().count(#p)),static_cast<p*>(MoverClassRegisterer::Entries()[#p]((tl))))
#define RegisteredMoverClasses (MoverClassRegisterer::Entries())
//static constructer
#define RegisterMoverClass(class_name) \
class class_name##_Registerer\
{\
public:\
	class_name##_Registerer()\
	{\
		MoverClassRegisterer::reg(#class_name, class_name##_Registerer::generate);\
	}\
	static Mover* generate(gfw::Tasklist* tl)\
	{\
		Mover* ptr = gfw::generate<class_name>(*tl);\
		ptr->setClassName(#class_name);\
		return ptr;\
	}\
	static class_name##_Registerer instance;\
};\
class_name##_Registerer class_name##_Registerer::instance

namespace jnr
{
	class Constants
	{
	public:
		static int time_invinc, time_stun;
		static int margin_camera_left;
		static bool debugging;
	};


	void newMover(void* ptr);
	class Character;
	enum eDamage{ Repelled = -2, Through = -1 };
	class Mover : public gfw::Object
	{
	public:
		Mover();
		//const gmtr::Geometry& Shape(){ return gmtr::NullGeometry::instance; }
		void setClassName(const char* str) { strncpy_s(class_name, str, 32); }
		virtual const gmtr::Geometry& Shape();
		virtual const gmtr::Rectangle& Rect();
		virtual const gmtr::Rectangle& Rect(const phsc::Physics& phys);
		virtual void initialize(int refx, int refy
			, gmtr::Vector2D p, OWNERHANDLE owner);
		virtual void finalize();
		virtual void run(jnr::JumpnRunData& data);
		virtual void render(gfw::BaseMedia& media, JumpnRunData& data){}
		gmtr::Vector2D doPhysics(JumpnRunData& data);
		virtual void drive(phsc::Physics& phys, jnr::JumpnRunData& data){}
		virtual void interact(jnr::Character& chara, jnr::JumpnRunData& data){}
		virtual gmtr::Vector2D react(const gmtr::Vector2D& v_prev, const gmtr::Vector2D& v_next, bool up)
		{
			return gmtr::Vector2D(0, 0);
		}
		virtual void stamping();
		bool pushTo(const gmtr::Vector2D& p);
		const gmtr::Vector2D& P()const{ return phys.p; }
		const gmtr::Vector2D& V()const{ return phys.v; }
		void P(const gmtr::Vector2D& v){ phys.p = v; }
		void V(const gmtr::Vector2D& v){ phys.v = v; }
		gmtr::Vector2D Center()const{ return phys.p + gmtr::Vector2D(width / 2, height / 2); }
		bool Turned()const{ return turned; }
		void Turned(bool b){ turned = b; }
		void GoOutOfCamera(bool b){ this->go_out_of_camera = b; }
		void Visible(bool b){ visible = b; }
		bool Visible() { return visible; }
		bool IsOnSomething() { return isOnSomething; }
		bool InteractingWithBlocks(){ return interacting_with_blocks; }
		OWNERHANDLE Owner()const { return owner; }
		OWNERHANDLE OwnerOriginal()const { return owner_original; }
		void Owner(OWNERHANDLE owner) {  this->owner = owner; }
		int ID(){ return id; }
		void Smash(const gmtr::Vector2D& v){ v_smash = v; }
		static jnr::Stage* stg;
	protected:
		static int total_mover_num;
		char class_name[32];
		int id;
		int age;
		int refx, refy;
		int width, height;
		int margin_side, margin_top, margin_bottom;
		bool turned, touch_and_dissapear, interacting_with_blocks, interacting_with_stages
			, smashable, go_out_of_camera, collided_with_earth, visible;
		int isOnSomething;
		char location;
		double smash_dimrate;
		gmtr::Rectangle rect;
		OWNERHANDLE owner, owner_original;
		gmtr::Vector2D v_smash, v_reaction;
	};

	class MoverClassRegisterer
	{
	public:
		static std::map<std::string, Mover* (*)(gfw::Tasklist* tl)>& Entries() { return *entries; }

		static std::map<std::string, Mover* (*)(gfw::Tasklist* tl)>* entries;
		static int initialized;
		static void reg(std::string s, Mover* (*gen)(gfw::Tasklist* tl))
		{
			static std::map<std::string, Mover* (*)(gfw::Tasklist* tl)> m;
			//m["null"] = nullptr;
			m[s] = gen;
			entries = &m;
		}
	};

};