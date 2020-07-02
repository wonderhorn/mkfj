#pragma once
#include"jumpnrun/ai/CharacterStatus.h"
#include<vector>
#include"jumpnrun/stage/Stage.h"
typedef std::vector<double> Feature;

namespace jnr
{
	//class Character;
	//class Character::eStatus;
	namespace ai
	{
		bool operator ==(const Feature& f1, const Feature& f2);
		bool operator >(const Feature& f1, const Feature& f2);
		bool operator <(const Feature& f1, const Feature& f2);
		//enum eStatus{ Standing, Walking, Dashing, Attacking, Casting, Damaged, Flying, Sitting };
		/*class CharacterStatus
		{
		public:
			int id;
			int age;
			int hp;
			int score;
			bool turned, isOnSomething;
			//::jnr::Character::eStatus status;
			eStatus status;
			gmtr::Vector2D p, v, v_reaction;
			const jnr::Stage* stg;
			gfw::Tasklist* tl;
			gfw::Object::Pipe pipe;
		};*/

		class FeatureExtractor
		{
		public:
			std::string format();
			Feature extract(const CharacterStatus& cs)const;
		};

		class RichFeatureExtractor
		{
		public:
			std::string format();
			Feature extract(const CharacterStatus& cs)const;
		};
	};
};