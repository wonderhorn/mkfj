#pragma once
#include<string>
#include<vector>
#include<deque>
#include"framework/VirtualController.h"
#include"jumpnrun/ai/FeatureExtractor.h"
namespace jnr
{
	namespace ai
	{
		class AIActionDecoder
	    {
		public:
			AIActionDecoder();
			bool act(gfw::ManualController& mc);
			void push(int code, gfw::ManualController& mc, const CharacterStatus& cs);
			const std::vector<int>& Symbols(){ return symbols; }
			std::vector<int> symbols;

			class Command
			{
			public:
				Command(): limit(0), button(0), age(0) {}
				Command(int limit, char button) : limit(limit), button(button), age(0) {}
				int age, limit;
				char button;
			};
			/*class ActionController
			{
			public:
				ActionController();
				//Command act(eAction action, const CharacterStatus& cs);
				void act(std::deque<Command>& commands, eAction action, const CharacterStatus& cs);
			private:
				bool turned;
			};
			ActionController action_cont;*/
			std::deque<Command> commands;

		private:

		};
	};
};