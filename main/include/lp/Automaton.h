#pragma once
#include<vector>
#include<map>
namespace lp
{
	typedef int InputType;
	typedef int StateId;
	class Automaton
	{
	public:
		Automaton();
		StateId Next(InputType input);
		bool isFin();
		bool Error(){return error;}
		void addFinState(StateId id);
		void addTransition(StateId ori, StateId ter, InputType input);
		StateId CurrnetState(){return current_state;}
		static const int ALL_STATES = -1;

	private:
		class State
		{
			friend class Automaton;
		public:
			State(StateId id);
			StateId Id(){return id;}
			StateId Next(InputType input);

		private:
			int id;
			std::map<InputType, StateId> next;
		};

		bool error;
		int num_state;
		StateId start_state;
		StateId current_state;
		std::vector<State> states;
		std::vector<StateId> fin_states;
		std::map<InputType, StateId> all_next;
	};
};