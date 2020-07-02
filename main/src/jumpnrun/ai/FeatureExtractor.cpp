#include"jumpnrun/ai/FeatureExtractor.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/ai/Radar.h"
#include"jumpnrun/mover/StageAccessor.h"
#include"utils/MyCppUtil.h"
#include<iostream>
using namespace gmtr;
using namespace gfw;
using namespace jnr;
using namespace ai;

std::string FeatureExtractor::format()
{
	return "#Hand-made feature extractor v1.0\n"
		"#turned, standing, walking, dashing, flying, wall_24, radar_16_4, radar_16_2";
}

Feature FeatureExtractor::extract(const CharacterStatus& cs)const
{
	Feature f;
	Character* ch = (Character*)(cs.pipe.Dst(*cs.tl));
	if (ch)
	{
		double absv = cs.v.l2();
		double d2wall = StageAccessor(*cs.stg).distanceToWall(cs.p, cs.turned, *cs.data);
		Character::eStatus status = (Character::eStatus)cs.status;
		f.push_back(ch->Turned());
		f.push_back(status == Character::eStatus::Standing);
		f.push_back(status == Character::eStatus::Walking);
		f.push_back(status == Character::eStatus::Dashing);
		f.push_back(status == Character::eStatus::Flying);
		f.push_back(d2wall <= 24);
		//std::cout << cs.v_reaction.x << std::endl;

		Radar radar;
		radar.myid = ch->ID();
		radar.myp = ch->P();
		radar.tl = cs.tl;
		radar.turned = cs.turned;
		auto vr = radar.scan(64 * 4);
		$for_i(vr.size())
		{
			Mover* mvr = vr[i];
			f.push_back(mvr != NULL);
		}
		vr = radar.scan(64 * 2);
		$for_i(vr.size())
		{
			Mover* mvr = vr[i];
			f.push_back(mvr != NULL);
		}
	}
	return f;
}


bool jnr::ai::operator ==(const Feature& f1, const Feature& f2)
{
	assert(f1.size() == f2.size());
	for (int i = 0; i < f1.size(); i++)
	{
		if (f1[i] != f2[i])
			return false;
	}
	return true;
}

bool jnr::ai::operator >(const Feature& f1, const Feature& f2)
{
	assert(f1.size() == f2.size());
	for (int i = 0; i < f1.size(); i++)
	{
		if (f1[i] > f2[i])
			return true;
		else if (f1[i] < f2[i])
			return false;
	}
	return false;
}

bool jnr::ai::operator <(const Feature& f1, const Feature& f2)
{
	assert(f1.size() == f2.size());
	for (int i = 0; i < f1.size(); i++)
	{
		if (f1[i] < f2[i])
			return true;
		else if (f1[i] > f2[i])
			return false;
	}
	return false;
}