#include"jumpnrun/MoverUtilities.h"
#include<deque>
#include<map>
using namespace gfw;
using namespace jnr;

EnemySearcher::EnemySearcher() : tl(nullptr), me(nullptr) {}

void EnemySearcher::initialize(jnr::Mover* me)
{
	this->me = me;
	tl = me->tl;
}

std::vector<gfw::Object::Pipe> EnemySearcher::search(double range)
{
	std::vector<gfw::Object::Pipe> rtn;
	for (auto t = tl->begin(); t != tl->end(); t++)
	{
		Object* obj = getObject(t);
		if (!obj->isMover)
			continue;
		Mover* mvr = static_cast<Mover*>(obj);
		if (mvr->ID() != me->ID())
		{
			auto dp = mvr->Center() - me->Center();
			if (dp.l2() <= range)
			{
				rtn.push_back(Object::Pipe(obj, *tl));
			}
		}
	}
	return rtn;
}

#define $PAIR std::pair<int, int>

Router::Router() : w(10), h(10), margin(5)
{
}

gmtr::Vector2D Router::grid2point(int x, int y, const gmtr::Vector2D& p_ori, const gmtr::Vector2D& p_ter)
{
	double gw = (p_ori.x - p_ter.x) / (w + margin * 2);
	double gh = (p_ori.y - p_ter.y) / (h + margin * 2);
	return gmtr::Vector2D(p_ter.x + gw * x, p_ter.y + gh * y);
}

std::pair<int, int> Router::point2grid(const gmtr::Vector2D& p, const gmtr::Vector2D& p_ori, const gmtr::Vector2D& p_ter)
{
	double gw = (p_ori.x - p_ter.x) / (w + margin * 2);
	double gh = (p_ori.y - p_ter.y) / (h + margin * 2);

	auto dp = p - p_ter;
	int x = dp.x / gw;
	int y = dp.y / gh;
	return $PAIR(x, y);
}

int Router::route(Stage& stage, const gmtr::Vector2D& p_ori, int myw, int myh, const gmtr::Vector2D& p_ter)
{
	auto grid_ori = point2grid(p_ori, p_ori, p_ter);
	auto grid_ter = point2grid(p_ter, p_ori, p_ter);
	grids.clear();
	grids.resize(h * w);
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			grids[i + j * w] = 9999999;
		}
	}

	std::deque<$PAIR> q;
	q.push_back($PAIR(w / 2, h / 2));

	while (!q.empty())
	{
		$PAIR p = q.front();
		$PAIR newp = p;
		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:
				newp.first++;
				break;
			case 1:
				newp.second++;
				break;
			case 2:
				newp.first--;
				break;
			case 3:
				newp.second--;
				break;

			default:
				break;
			}
			if (0 <= newp.first && newp.first < w
				|| 0 <= newp.second && newp.second < h)
			{
				double dp = grids[p.first + p.second * w];
				double dnewp = grids[newp.first + newp.second * w];
				if (dp > dnewp + 1)
				{
					grids[p.first + p.second * w] = dnewp + 1;
					q.push_back(newp);
				}
			}
		}
	}

	int argmin = -1;
	double min_d = 99999;
	for (int i = 0; i < 8; i++)
	{
		int x = 0, y = 0;
		switch (i)
		{
		case 0:
			x = 1;
			break;
		case 1:
			x = 1;
			y = 1;
			break;
		case 2:
			y = 1;
			break;
		case 3:
			x = -1;
			y = 1;
			break;
		case 4:
			x = -1;
			break;
		case 5:
			x = -1;
			y = -1;
			break;
		case 6:
			y = -1;
			break;
		case 7:
			x = 1;
			y = -1;
			break;
		}
		double d = grids[(std::get<0>(grid_ori) + x) + (std::get<1>(grid_ori) + y) * w];
		if (d < min_d)
		{
			min_d = d;
			argmin = i;
		}
	}
	return argmin;
}