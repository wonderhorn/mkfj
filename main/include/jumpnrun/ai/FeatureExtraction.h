#pragma once
#include"jumpnrun/ai/AI.h"
#include"jumpnrun/ai/CharacterStatus.h"

namespace jnr
{
	namespace ai
	{
		class FeatureExtraction
		{
		public:
			virtual Feature f(CharacterStatus cs) = 0;
			virtual double kernel(const Feature& f1, const Feature& f2);
		};
	};
};