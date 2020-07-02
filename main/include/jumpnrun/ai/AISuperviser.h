#pragma once
#include"jumpnrun/ai/CharacterStatus.h"
#include"jumpnrun/ai/FeatureExtractor.h"
#include"framework/VirtualController.h"
//#include"AI_Base.h"
#include<vector>

namespace jnr
{
	namespace ai
	{
		class AISuperviser
		{
		public:
			AISuperviser();
			void push(const CharacterStatus& cs, const gfw::VirtualController& vc);
			void push(const Feature& f, int score, int decision);
			void save(const std::string& filename)const;
			void clear(){ data.clear(); }
			void setHeader(const std::string& str);

			class DataElement
			{
			public:
				DataElement(const Feature& f, int score, int b)
					: f(f), score(score), b(b){}
				Feature f;
				int score;
				int b;
			};
			std::vector<DataElement> read(const std::string& filename)const;
		private:

			std::vector<DataElement> data;
			std::string header;
		};
	};
};