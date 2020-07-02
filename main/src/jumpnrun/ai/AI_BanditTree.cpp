#include"jumpnrun/ai/FeatureExtractor.h"
#include"jumpnrun/ai/AI_Bandit.h"
#include"jumpnrun/system/Parmanent.h"
#include"utils/string_util.h"
//#include<windows.h>
using namespace gmtr;
using namespace gfw;
using namespace jnr;
using namespace ai;

#ifndef GFW_Win33
void CreateDirectory(const char* file_path, void* ptr) {}
void GetCurrentDirectory(size_t bufsize, char* dst) {}
#endif

AI_BanditTree::Node::Node(const std::string& str)
{
	auto tokens = myutil::split(str, "_");
	i = myutil::str2int(tokens[0]);
	th = myutil::str2int(tokens[1]);
}

AI_BanditTree::AI_BanditTree() : delay_feedback(5)
{
	act2num.bin_width = 1.0;
	act2score.bin_width = 1.0;
}

AI_BanditTree::~AI_BanditTree(){}
void AI_BanditTree::initialize()
{
	//$P.aisuperviser.setHeader(RichFeatureExtractor().format());
	auto it = decision_tree.Root();
	it = decision_tree.push_back(it, Node(7, 32));
	decision_tree.push_back(it, Node(0, 0));
	//it.goToChild(1);
	decision_tree.push_back(it, Node(1, 0));
	for (int i = 0; i < 2; i++)
	{
		bandits.push_back(Bandit());
	}

	std::cout << "ai_bandit: " << std::endl;
	//std::cout << decision_tree << std::endl;
	decision_tree.print();
}

static void castTree(space_tree::tree<std::string>::iterator src
	, space_tree::tree<AI_BanditTree::Node>::iterator dst
	, space_tree::tree<AI_BanditTree::Node>& t)
{
	for (int i = 0; i < src.ChildNum(); i++)
	{
		AI_BanditTree::Node n;
		auto tmp = src;
		tmp.goToChild(i);
		auto v = myutil::split(*tmp, "_");
		if (v.size() == 2)
		{
			n.i = myutil::str2int(v[0]);
			n.th = myutil::str2double(v[1]);
		}
		auto newdst = t.push_back(dst, n);
		//newdst.goToChild(i);
		castTree(tmp, newdst, t);
	}
}

void enumLeaves
(space_tree::tree<AI_BanditTree::Node>& t, space_tree::tree<AI_BanditTree::Node>::iterator it
, std::vector<space_tree::tree<AI_BanditTree::Node>::iterator>& v)
{
	if (it.ChildNum() == 0)
		v.push_back(it);
	else
	{
		for (int i = 0; i
			< it.ChildNum(); i++)
		{
			auto newit = it;
			newit.goToChild(i);
			enumLeaves(t, newit, v);
		}
	}
}

void AI_BanditTree::initialize(space_tree::tree<std::string>& program)
{
	//$P.aisuperviser.setHeader(RichFeatureExtractor().format());
	//program.

	auto it = decision_tree.Root();
	auto it_src = program.Root();

	for (int i = 0; i < 100; i++)
	{
		bandits.push_back(Bandit());
	}
	decision_tree = space_tree::tree < Node > ();
	castTree(it_src, it, decision_tree);
	std::vector<space_tree::tree<AI_BanditTree::Node>::iterator> leaves;
	it = decision_tree.Root();
	enumLeaves(decision_tree, it, leaves);
	for (int i = 0; i < leaves.size(); i++)
	{
		leaves[i]->i = i;
	}

	std::cout << "ai_bandit: " << std::endl;
	decision_tree.print();
}
void AI_BanditTree::initialize(const std::string& src)
{
	//$P.aisuperviser.setHeader(RichFeatureExtractor().format());
	space_tree::tree<std::string> program;
	program.parse(src);
	auto it = decision_tree.Root();
	auto it_src = program.Root();

	for (int i = 0; i < 100; i++)
	{
		bandits.push_back(Bandit());
	}
	decision_tree = space_tree::tree<AI_BanditTree::Node>();
	castTree(it_src, it, decision_tree);


	std::cout << "ai_bandit: " << std::endl;
	decision_tree.print();
}

Feature AI_BanditTree::processMyStatus(CharacterStatus cs)
{
	return RichFeatureExtractor().extract(cs);
}

int AI_BanditTree::doTree(const Feature& f)
{
	auto it = decision_tree.Root();
	it.goToChild(0);
	while (it.ChildNum() > 0)
	{
		int d = it->i;
		int b = f[d] >= it->th;
		it.goToChild(b);
	}
	return it->i;
}

int AI_BanditTree::decide(const CharacterStatus& cs, const Feature& f)
{
	eAction action = (eAction)0;// = (eAction)($P.rand.get() % 8);
	double u = (double)$P.rand.get() / RAND_MAX;
	int index_bandit = doTree(f);
	Bandit& banit = bandits[index_bandit];

	if (u <= 0.2)
	{
		action = (eAction)($P.rand.get() % actdec.Symbols().size());
	}
	else
	{
		double s_max = -FLT_MAX;
		int argmax = 0;
		for (int i = 0; i < actdec.Symbols().size(); i++)
		{
			double s = banit.act2score.get(i);
			int n = banit.act2num.get(i);
			if (n == 0)
			{
				action = (eAction)i;
				break;
			}
			else
			{
				s /= n;
				if (s > s_max)
				{
					s_max = s;
					argmax = i;
				}
			}
		}
		action = (eAction)argmax;
	}

	q_score.push_back(cs.score);
	q_action.push_back(action);
	q_bandit.push_back(index_bandit);
	if (q_score.size() > delay_feedback)
	{
		double dscore = q_score.back() - q_score.front();
		Bandit& bandit_feedback = bandits[q_bandit.front()];
		bandit_feedback.act2score.add(q_action.front(), dscore);
		bandit_feedback.act2num.add(q_action.front(), 1);


		while (q_score.size() > delay_feedback)
		{
			q_score.pop_front();
			q_action.pop_front();
			q_bandit.pop_front();
		}
	}
	return action;
}

void AI_BanditTree::think(const CharacterStatus& cs)
{
	int action = 0;
	auto f = processMyStatus(cs);
	assert(f_prev.size() == 0 || f.size() == f_prev.size());
	bool empty = actdec.act(mc);
	if (empty)
	{
		action = decide(cs, f);
		actdec.push(action, mc, cs);
		//command.pushAction(action, cs);
	}
	//command.getCommand(this->mc, cs);

	$P.aisuperviser.push(f, cs.hp, action);
	f_prev = f;
}

bool writeString(const std::string& str, const std::string& file_name)
{
	std::ofstream ofs(file_name);
	if (!ofs)
	{
		return false;
	}
	ofs << str;
	return true;
}

bool AI_BanditTree::save(const std::string& path)
{
	CreateDirectory(path.c_str(), (void*)NULL);
	char cd[10240];
	GetCurrentDirectory((size_t)10240, cd);
	//SetCurrentDirectory("..\\");
	decision_tree.printFile(path + "tree.txt");
	for (int i = 0; i < bandits.size(); i++)
	{
		auto a2s = bandits[i].act2score.toString();
		writeString(a2s, path + "act2score_" + myutil::int2str(i) + ".txt");
		auto a2n = bandits[i].act2num.toString();
		writeString(a2n, path + "act2num_" + myutil::int2str(i) + ".txt");
	}
	return true;
}

bool AI_BanditTree::load(const std::string& path)
{
	decision_tree.load(path + "tree.txt");
	bandits.clear();
	for (int i = 0; true; i++)
	{
		bandits.push_back(Bandit());
		auto& bnd = bandits[bandits.size() - 1];
		bool b1 = bnd.act2score.read(path + "act2score_" + myutil::int2str(i) + ".txt");
		bool b2 = bnd.act2num.read(path + "act2num_" + myutil::int2str(i) + ".txt");
		if (!b1 || !b2)
			break;
	}
	return true;
}

std::ostream& operator << (std::ostream& ost, const jnr::ai::AI_BanditTree::Node& n)
{
	ost << n.i << "_" << n.th;
	return ost;
}