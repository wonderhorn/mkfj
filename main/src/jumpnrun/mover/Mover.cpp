#include"jumpnrun/mover/Mover.h"
#include"jumpnrun/mover/effect/Effect.h"
#include"jumpnrun/GRAPHICS.h"
#include"jumpnrun/system/Parmanent.h"
#include<iostream>
using namespace jnr;
using namespace gfw;
using namespace gmtr;
#define $V Vector2D

Stage* Mover::stg = NULL;
int Mover::total_mover_num = 0;
std::map<std::string, Mover* (*)(gfw::Tasklist* tl)> *MoverClassRegisterer::entries = nullptr;
int MoverClassRegisterer::initialized = 123456;


static bool mergeReaction($V& v1, const $V& v2)
{
	if (v1.x * v2.x < 0)
	{
		v1.x = 0.0;
		return false;
	}
	else if (fabs(v1.x) < fabs(v2.x))
	{
		v1.x = v2.x;
	}

	if (v1.y * v2.y < 0)
	{
		v1.y = 0.0;
		return false;
	}
	else if (fabs(v1.y) < fabs(v2.y))
	{
		v1.y = v2.y;
	}
	return true;
}

Mover::Mover(){}

void Mover::initialize(int refx, int refy, gmtr::Vector2D p, OWNERHANDLE owner)
{
	Object::initialized();
	isMover = true;
	this->owner = owner;
	this->owner_original = owner;
	id = total_mover_num;
	total_mover_num++;
	this->phys.p = p;
	this->phys.v = this->phys.a = Vector2D(0.0, 0.0);
	age = 0;
	this->refx = refx;
	this->refy = refy;
	width = height = 0;
	margin_side = margin_top = margin_bottom = 0;
	turned = false;
	isOnSomething = false;
	touch_and_dissapear = false;
	interacting_with_blocks = false;
	interacting_with_stages = true;
	smashable = true;
	collided_with_earth = false;
	go_out_of_camera = true;
	visible = true;
	v_smash.x = v_smash.y = 0.0;
	smash_dimrate = 1.2;
	location = 'n';
	writeName("mover", name);
}
void Mover::finalize()
{
	alive = false;
}

const gmtr::Geometry& Mover::Shape()
{
	rect.x1 = phys.p.x + margin_side;
	rect.x2 = phys.p.x + width - margin_side;
	rect.y1 = phys.p.y + margin_top;
	rect.y2 = phys.p.y + height - margin_bottom;
	return rect;
}
const gmtr::Rectangle& Mover::Rect()
{
	rect.x1 = phys.p.x + margin_side;
	rect.x2 = phys.p.x + width - margin_side;
	rect.y1 = phys.p.y + margin_top;
	rect.y2 = phys.p.y + height;
	return rect;
}
const gmtr::Rectangle& Mover::Rect(const phsc::Physics& phys)
{
	rect.x1 = phys.p.x + margin_side;
	rect.x2 = phys.p.x + width - margin_side;
	rect.y1 = phys.p.y + margin_top;
	rect.y2 = phys.p.y + height;
	return rect;
}

void Mover::run(jnr::JumpnRunData& data)
{
	v_reaction = doPhysics(data);
	//std::cout << v_reaction.x << std::endl;
	age++;
}

void Mover::stamping()
{
	isOnSomething = 5;
	this->phys.v.y = 0;
}

bool Mover::pushTo(const gmtr::Vector2D& p)
{
	auto newphys = this ->phys;
	auto r = Rect();
	$V reaction(0, 0);
	auto v = p - phys.p;
	if (interacting_with_blocks)
	{
		for (auto t = tl->begin(); t != tl->end(); t++)
		{
			Object* obj = getObject(t);
			if (obj->isMover)
			{
				Mover* mvr = (Mover*)obj;
				if (this->ID() == mvr->ID())
					continue;
				mergeReaction(reaction, mvr->react(Vector2D(r.x1, r.y1)
					, Vector2D(r.x1, r.y1) + v, true));
				mergeReaction(reaction, mvr->react(Vector2D(r.x2, r.y1)
					, Vector2D(r.x2, r.y1) + v, true));
				mergeReaction(reaction, mvr->react(Vector2D(r.x2, r.y2)
					, Vector2D(r.x2, r.y2) + v, false));
				mergeReaction(reaction, mvr->react(Vector2D(r.x1, r.y2)
					, Vector2D(r.x1, r.y2) + v, false));
				mergeReaction(reaction, mvr->react(Vector2D((r.x1 + r.x2) / 2, r.y2)
					, Vector2D((r.x1 + r.x2) / 2, r.y2) + phys.v, false));
				mergeReaction(reaction, mvr->react(Vector2D(r.x1, (r.y1 + 3 * r.y2) / 4)
					, Vector2D(r.x1, (r.y1 + 3 * r.y2) / 4) + v, false));
				mergeReaction(reaction, mvr->react(Vector2D(r.x2, (r.y1 + 3 * r.y2) / 4)
					, Vector2D(r.x2, (r.y1 + 3 * r.y2) / 4) + v, false));
			}
		}
	}
	v = p - phys.p + reaction;
 	r = Rect();

	if (stg->enterable($V(r.x1, r.y1), $V(r.x1, r.y1) + v) &&
		stg->enterable($V(r.x2, r.y1), $V(r.x2, r.y1) + v) &&
		stg->enterable($V(r.x2, r.y2), $V(r.x2, r.y2) + v) &&
		stg->enterable($V(r.x1, r.y2), $V(r.x1, r.y2) + v))
	//if (stg->enterable(phys.p, p))
		//phys.p = p;
		phys.p += v;

	r = Rect();
	return 0;
}

gmtr::Vector2D Mover::doPhysics(JumpnRunData& data)
{
	collided_with_earth = false;
	Vector2D rtn = Vector2D(0,0);
	Shape();
	const gmtr::Rectangle& r = Rect();
	bool OK = true;

	//$V vx(phys.v.x, 0.0);
	$V reaction(0.0, 0.0);
	if (interacting_with_stages)
	{
		mergeReaction(reaction, stg->react(Vector2D(r.x1, r.y1)
			, Vector2D(r.x1, r.y1) + phys.v, true));
		mergeReaction(reaction, stg->react(Vector2D(r.x2, r.y1)
			, Vector2D(r.x2, r.y1) + phys.v, true));

		mergeReaction(reaction, stg->react(Vector2D(r.x2, r.y2)
			, Vector2D(r.x2, r.y2) + phys.v, false));
		mergeReaction(reaction, stg->react(Vector2D(r.x1, r.y2)
			, Vector2D(r.x1, r.y2) + phys.v, false));

		mergeReaction(reaction, stg->react(Vector2D(r.x1, (r.y1 + r.y2) / 2)
			, Vector2D(r.x1, (r.y1 + r.y2) / 2) + phys.v, false));
		mergeReaction(reaction, stg->react(Vector2D(r.x2, (r.y1 + r.y2) / 2)
			, Vector2D(r.x2, (r.y1 + r.y2) / 2) + phys.v, false));

	}

	if (std::string(this->name) == "character_player")
	{
		int a = 0;
	}
	if (interacting_with_blocks)
	{
		for (auto t = tl->begin(); t != tl->end(); t++)
		{
			Object* obj = getObject(t);
			if (obj->isMover)
			{
				Mover* mvr = (Mover*)obj;
				if (this->ID() == mvr->ID())
					continue;
				
				mergeReaction(reaction, mvr->react(Vector2D(r.x1, r.y1)
					, Vector2D(r.x1, r.y1) + phys.v, true));
				mergeReaction(reaction, mvr->react(Vector2D(r.x2, r.y1)
					, Vector2D(r.x2, r.y1) + phys.v, true));
				mergeReaction(reaction, mvr->react(Vector2D(r.x2, r.y2)
					, Vector2D(r.x2, r.y2) + phys.v, false));
				mergeReaction(reaction, mvr->react(Vector2D(r.x1, r.y2)
					, Vector2D(r.x1, r.y2) + phys.v, false));
				mergeReaction(reaction, mvr->react(Vector2D((r.x1 + r.x2) / 2, r.y1)
					, Vector2D((r.x1 + r.x2) / 2, r.y1) + phys.v, true));

				mergeReaction(reaction, mvr->react(Vector2D((r.x1 + r.x2)/2, r.y2)
					, Vector2D((r.x1 + r.x2) / 2, r.y2) + phys.v, false));
				mergeReaction(reaction, mvr->react(Vector2D(r.x1, (r.y1 + 3 * r.y2) / 4)
					, Vector2D(r.x1, (r.y1 + 3 * r.y2) / 4) + phys.v, false));
				mergeReaction(reaction, mvr->react(Vector2D(r.x2, (r.y1 + 3 * r.y2) / 4 )
					, Vector2D(r.x2, (r.y1 + 3 * r.y2) / 4 ) + phys.v, false));
				mergeReaction(reaction, mvr->react(Vector2D(r.x1, (3 * r.y1 + r.y2) / 4)
					, Vector2D(r.x1, (3 * r.y1 + r.y2) / 4) + phys.v, false));
				mergeReaction(reaction, mvr->react(Vector2D(r.x2, (3 * r.y1 + r.y2) / 4)
					, Vector2D(r.x2, (3 * r.y1 + r.y2) / 4) + phys.v, false));
			}
		}
	}

	if (!go_out_of_camera)
	{
		auto v1 = Vector2D(r.x1, r.y1) + phys.v;
		auto v_x = -$V(phys.v.x, 0);
		const int camera_x = data.camera.X() + Constants::margin_camera_left;
		if (v1.x < camera_x || v1.x > data.camera.X() + data.camera.Width())
		{
			mergeReaction(reaction, v_x);
		}
		auto v2 = Vector2D(r.x2, r.y1) + phys.v;
		if (v2.x < camera_x || v2.x > data.camera.X() + data.camera.Width())
		{
			mergeReaction(reaction, v_x);
		}
		auto v3 = Vector2D(r.x2, r.y2) + phys.v;
		if (v3.x < camera_x || v3.x > data.camera.X() + data.camera.Width())
		{
			mergeReaction(reaction, v_x);
		}
		auto v4 = Vector2D(r.x1, r.y2) + phys.v;
		if (v4.x < camera_x || v4.x > data.camera.X() + data.camera.Width())
		{
			mergeReaction(reaction, v_x);
		}
	}

	isOnSomething--;
	if (isOnSomething < 0)
	{
		isOnSomething = 0;
	}
	/*if (reaction.y < 0)
	{
		stamping();
	}*/

	phsc::Physics newphys = phys;

	newphys.v += reaction;
	/*if (reaction.x != 0.0)
		newphys.v.x = 0.0;
	if (reaction.y != 0.0)
		newphys.v.y = 0.0;*/
	newphys.p += newphys.v;
	//newphys.p += newphys.v + v_smash + reaction;
	newphys.v += newphys.a;

	double max_vy = 6.0;
	if (location == 'w')
		max_vy = 2.5;
	if (newphys.v.y >= max_vy)
		newphys.v.y = max_vy;
	drive(newphys, data);
	if (isOnSomething < 5)
	{
		if(location == 'w' || stg->getType(Center() + Vector2D(0, 24)) == 'w')
			newphys.a = Vector2D(0.0, 0.08 * phys.mass);
		else
			newphys.a = Vector2D(0.0, 0.2 * phys.mass);
	}
	if (reaction.y < 0)
		stamping();
	//if (fabs(v_smash.x) <= 0.3 && fabs(v_smash.y) <= 0.3)
	phys = newphys;
	if (touch_and_dissapear && (reaction.x != 0.0 || reaction.y != 0.0))
	{
		collided_with_earth = true;
		generate<Effect>(*tl)->initialize(REFPOINT_REPELED_X, REFPOINT_REPELED_Y, 32, 32
			, newphys.p + $V((this->width - 32) / 2, (this->height - 32) / 2), 4,
			$V(0.0, 0.0), 0);
		generate<Effect>(*tl)->initialize(REFPOINT_REPELED_X, REFPOINT_REPELED_Y + 32, 32, 32
			, newphys.p + $V((this->width - 32) / 2, (this->height - 32) / 2),
			6, $V(0.0, 0.0), 0);
		finalize();
	}
	rtn = reaction;

	//update location
	location = stg->getType(Center());

	//smash
	if (smashable && !(v_smash.x == 0.0 && v_smash.y == 0.0))
	{
		Shape();
		reaction = $V(0.0, 0.0);
		if (interacting_with_stages)
		{
			mergeReaction(reaction, stg->react(Vector2D(r.x1, r.y1)
				, Vector2D(r.x1, r.y1) + v_smash, true));
			mergeReaction(reaction, stg->react(Vector2D(r.x2, r.y1)
				, Vector2D(r.x2, r.y1) + v_smash, true));
			mergeReaction(reaction, stg->react(Vector2D(r.x2, r.y2)
				, Vector2D(r.x2, r.y2) + v_smash, false));
			mergeReaction(reaction, stg->react(Vector2D(r.x1, r.y2)
				, Vector2D(r.x1, r.y2) + v_smash, false));
		}
		if (interacting_with_blocks)
		{
			for (auto t = tl->begin(); t != tl->end(); t++)
			{
				Object* obj = getObject(t);
				if (obj->isMover)
				{
					Mover* mvr = (Mover*)obj;
					if (this->ID() == mvr->ID())
						continue;
					mergeReaction(reaction, mvr->react(Vector2D(r.x1, r.y1)
						, Vector2D(r.x1, r.y1) + v_smash, true));
					mergeReaction(reaction, mvr->react(Vector2D(r.x2, r.y1)
						, Vector2D(r.x2, r.y1) + v_smash, true));
					mergeReaction(reaction, mvr->react(Vector2D(r.x2, r.y2)
						, Vector2D(r.x2, r.y2) + v_smash, false));
					mergeReaction(reaction, mvr->react(Vector2D(r.x1, r.y2)
						, Vector2D(r.x1, r.y2) + v_smash, false));
					mergeReaction(reaction, mvr->react(Vector2D((r.x1 + r.x2) / 2, r.y2)
						, Vector2D((r.x1 + r.x2) / 2, r.y2) + v_smash, false));
					mergeReaction(reaction, mvr->react(Vector2D(r.x1, (r.y1 + 3 * r.y2) / 4)
						, Vector2D(r.x1, (r.y1 + 3 * r.y2) / 4) + v_smash, false));
					mergeReaction(reaction, mvr->react(Vector2D(r.x2, (r.y1 + 3 * r.y2) / 4)
						, Vector2D(r.x2, (r.y1 + 3 * r.y2) / 4) + v_smash, false));
				}
			}
		}

		if (!go_out_of_camera)
		{
			auto v1 = Vector2D(r.x1, r.y1) + v_smash;
			const int camera_x = data.camera.X() + Constants::margin_camera_left;
			//const int camera_x = data.camera.X() + 48;
			if (v1.x < camera_x || v1.x > data.camera.X() + data.camera.Width())
			{
				mergeReaction(reaction, -v_smash);
			}
			auto v2 = Vector2D(r.x2, r.y1) + v_smash;
			if (v2.x < camera_x || v2.x > data.camera.X() + data.camera.Width())
			{
				mergeReaction(reaction, -v_smash);
			}
			auto v3 = Vector2D(r.x2, r.y2) + v_smash;
			if (v3.x < camera_x || v3.x > data.camera.X() + data.camera.Width())
			{
				mergeReaction(reaction, -v_smash);
			}
			auto v4 = Vector2D(r.x1, r.y2) + v_smash;
			if (v4.x < camera_x || v4.x > data.camera.X() + data.camera.Width())
			{
				mergeReaction(reaction, -v_smash);
			}
		}

		v_smash += reaction;
		v_smash = v_smash / smash_dimrate;
		phys.p += v_smash;
		rtn += reaction;
	}
	return rtn;
}