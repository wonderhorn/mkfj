#include"jumpnrun/mover/Barrier.h"
#include"jumpnrun/system/Parmanent.h"
using namespace gfw;
using namespace jnr;

RegisterMoverClass(Wing);

const int NUM_JUMP = 5;
const int COST_MP = 2;

void Wing::initialize(int refx, int refy
	, gmtr::Vector2D p, OWNERHANDLE owner, Character* chara)
{
	Mover::initialize(refx, refy, p, owner);
	pipe = Object::Pipe(chara, *tl);
	chara->setBarrier(this);
	lifetime = 60 * 30;
	width = height = 32;
	max_num_jump_ori = chara->MaxNumJump();
	jump_const_mp_ori = chara->JumpCostMP();
	chara->MaxNumJump(max_num_jump_ori + NUM_JUMP);
	chara->JumpCostMP(jump_const_mp_ori + COST_MP);
	writeName("barrier_wing", name);
}
void Wing::run(jnr::JumpnRunData& data)
{
	Mover::run(data);
	if (!pipe.Dst(*tl) || !pipe.Dst(*tl)->alive)
	{
		die(data);
		return;
	}
	else
	{
		Character* c = static_cast<Character*>(pipe.Dst(*tl));
		phys.p = c->Center() - gmtr::Vector2D(width / 2, height / 2);
		//c->MaxNumJump(max_num_jump_ori5);
		//c->JumpCostMP(2);

	}
	if (lifetime >= 0 && age > lifetime)
		die(data);
}
void Wing::render(gfw::BaseMedia& media, jnr::JumpnRunData& data)
{
	BaseGraphics* graphics = media.getGraphics();
	const int offset_y = 4;
	int offset_refy = 1;
	if (pipe.Dst(*tl))
	{
		Character* c = static_cast<Character*>(pipe.Dst(*tl));
		if (!c->onSomething())
			offset_refy = age / 20 % 2;
		if (c->Status() == c->Fallen)
			return;
	}
	graphics->draw($P.img_obj, phys.p.x - 20 - data.camera.X(), phys.p.y + offset_y - data.camera.Y(), 0
		, refx, refy + offset_refy * height, width, height, false, 0xffffffff);
	graphics->draw($P.img_obj, phys.p.x + 20 - data.camera.X(), phys.p.y + offset_y - data.camera.Y(), 0
		, refx, refy + offset_refy * height, width, height, true, 0xffffffff);
}
int Wing::damage(int power, int stun_time, int invinc_time)
{
	if (!alive)
	{
		return -1;
	}
	//die(data);
	lifetime -= power * 24;
	return eDamage::Through;
}
int Wing::damage(const Damage& d)
{
	if (!alive)
	{
		return -1;
	}
	//die();
	lifetime -= d.power * 24;

	return eDamage::Through;
}
void Wing::cancel()
{
	//die();
	lifetime = 0;
}
void Wing::die(jnr::JumpnRunData& data)
{
	if (auto* ptr = pipe.Dst(*tl))
	{
		Character* c = static_cast<Character*>(ptr);
		max_num_jump_ori = c->MaxNumJump();
		jump_const_mp_ori = c->JumpCostMP();
		c->setBarrier(nullptr);
		c->MaxNumJump(max_num_jump_ori - NUM_JUMP);
		c->JumpCostMP(jump_const_mp_ori - COST_MP);
	}
	alive = false;

}