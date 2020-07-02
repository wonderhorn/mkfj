#pragma once
#include<vector>
#include"framework/Object.h"
//#include"Character.h"
#include"jumpnrun/stage/Stage.h"
#include"utils/OwnerHandle.h"
#include"jumpnrun/system/JumpnRunData.h"
namespace jnr
{
	namespace ai
	{
		enum eStatus{ Standing, Walking, Dashing, Attacking, Casting, Damaged, Flying, Sitting };
		class CharacterStatus
		{
		public:
			int id;
			int age;
			int hp, mp;
			int score;
			int num_of_friends;
			char location;
			bool turned, isOnSomething;
			OWNERHANDLE owner;
			//::jnr::Character::eStatus status;
			eStatus status;
			gmtr::Vector2D p, v, v_reaction;
			const jnr::Stage* stg;
			gfw::Tasklist* tl;
			gfw::Object::Pipe pipe;
			JumpnRunData* data;
		};
	};
};