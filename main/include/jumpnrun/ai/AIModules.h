#pragma once
#include"jumpnrun/ai/AI_Base.h"

namespace jnr
{
	namespace ai
	{
		class ConstModule : public AIModule
		{
		public:
			ConstModule(Decision d) : d(d){}
			Decision decide(const CharacterStatus& cs) { return d; }
			Decision d;
		};

		class Alternate : public AIModule
		{
		public:
			Alternate(AIModule* m1, AIModule* m2) : m1(m1), m2(m2), counter(0){}
			Decision decide(const CharacterStatus& cs) { return (counter++) % 2 ? m2->decide(cs) : m1->decide(cs); }
			AIModule* m1, *m2;
			char counter;
		};

		class Switch : public AIModule
		{
		public:
			Switch(int f, AIModule* m1, AIModule* m2) : f(f), m1(m1), m2(m2){}
			Decision decide(const CharacterStatus& cs) { return f ? m2->decide(cs) : m1->decide(cs); }
			AIModule* m1, *m2;
			int f;
		};
	};
};