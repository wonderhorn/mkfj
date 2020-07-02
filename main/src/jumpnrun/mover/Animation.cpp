#include"framework/Animation.h"
#include<assert.h>
using namespace jnr;

Animation::Animation() : finished(false), spell(spl::eSpell::spl_nospell){}

int Animation::RefX()
{
	int offset = 0;
	if (interval == 0 || finished)
		offset = 0;
	else if (counter >= num_frame * interval * num_iteration)
		offset = (num_frame - 1);
	else offset = (counter / interval) % num_frame;

	return refx + offset * w;
}
int Animation::RefY()
{
	return refy;
}
int Animation::Frame()
{
	int offset = 0;
	if (interval == 0 || finished)
		offset = 0;
	else if (counter >= num_frame * interval * num_iteration)
		offset = (num_frame - 1);
	else offset = (counter / interval) % num_frame;

	return offset;
}
int Animation::Duration()
{
	return num_frame * interval * num_iteration + last_frame_time;
}
void Animation::Advance()
{
	if (interval == 0 || counter+1 >= num_frame * interval * num_iteration + last_frame_time)
	{
		finished = true;
		interval = 0;
		spell = spl::eSpell::spl_nospell;
	}
	counter++;
}

void Animation::reset(int refx, int refy, int interval
	, int num_frame, int w, int h, int last_frame_time, int num_iteration)
{
	this->refx = refx;
	this->refy = refy;
	this->interval = interval;
	this->num_frame = num_frame;
	this->w = w;
	this->h = h;
	this->last_frame_time = last_frame_time;
	this->num_iteration = num_iteration;
	finished = false;
	counter = 0;
	spell_time = -1;
	spell = spl::eSpell::spl_nospell;
}

void Animation::setSpell(jnr::spl::eSpell spell, int spell_time)
{
	this->spell = spell;
	this->spell_time = spell_time;
	assert(num_frame * interval * num_iteration + last_frame_time >= spell_time);
}