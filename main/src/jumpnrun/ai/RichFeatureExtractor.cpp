#include"jumpnrun/ai/FeatureExtractor.h"
#include"jumpnrun/system/Parmanent.h"
#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/ai/Radar.h"
#include"jumpnrun/mover/StageAccessor.h"
#include"utils/MyCppUtil.h"
#include"utils/string_util.h"
#include<iostream>
using namespace gmtr;
using namespace gfw;
using namespace jnr;
using namespace ai;

std::string RichFeatureExtractor::format()
{
	return "#Rich feature extractor v1.0";
}

static void mover2feature(Mover* mvr, Feature& f, const CharacterStatus& cs)
{
	if (!mvr)
	{
		f.push_back(0.0);
		f.push_back(0.0);
		f.push_back(0.0);
		f.push_back(0.0);
		f.push_back(0.0);
		f.push_back(0.0);
		f.push_back(0.0);
		//f.push_back(mvr->);
		f.push_back(0.0);
	}
	else
	{
		Character* ch = (Character*)(cs.pipe.Dst(*cs.tl));
		int sign = cs.turned ? -1 : 1;
		f.push_back(1.0);
		f.push_back(myutil::find(mvr->Name(), 0, -1, "bullet") >= 0);
		f.push_back(myutil::find(mvr->Name(), 0, -1, "enemy") >= 0);
		f.push_back((mvr->Center().x - cs.p.x) * sign / 480);
		f.push_back((mvr->Center().y - cs.p.y) / 480);
		f.push_back(mvr->V().x / 32);
		f.push_back(mvr->V().y / 32);
		//f.push_back(mvr->);
		f.push_back(mvr->Owner() == ch->Owner());
	}
}

Feature RichFeatureExtractor::extract(const CharacterStatus& cs)const
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
		f.push_back(cs.v.x / 32);
		f.push_back(cs.v.y / 32);
		f.push_back(d2wall / 480);
		//std::cout << cs.v_reaction.x << std::endl;

		Radar radar;
		radar.myid = ch->ID();
		radar.myp = cs.p;
		radar.tl = cs.tl;
		radar.turned = cs.turned;
		auto vr = radar.scan(64 * 4);
		$for_i(vr.size())
		{
			Mover* mvr = vr[i];
			mover2feature(mvr, f, cs);
		}
	}
	return f;
}