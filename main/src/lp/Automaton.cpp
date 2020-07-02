#include"lp/Automaton.h"
#include<iostream>

using namespace lp;

Automaton::Automaton(): start_state(0), current_state(0), num_state(1), error(false)
{
	states.push_back(State(0));
}

StateId Automaton::Next(InputType input)
{
	//return current_state = states[current_state].Next(input);
	if(states[current_state].next.count(input))
		return (current_state = states[current_state].next[input]);
	else if(all_next.count(input))
		return (current_state = all_next[input]);
	else
	{
		error = true;
		return current_state;
	}
}

void Automaton::addFinState(StateId id)
{
	fin_states.push_back(id);
}

void Automaton::addTransition(StateId ori, StateId ter, InputType input)
{
	if(ori >= num_state)
	{
		for(int i = num_state; i <= ori; i++)
		{
			states.push_back(State(i));
			num_state++;
		}
	}

	if(ter >= num_state)
	{
		for(int i = num_state; i <= ter; i++)
		{
			states.push_back(State(i));
			num_state++;
		}
	}

	if(ori != ALL_STATES)
		states[ori].next.insert(std::map<InputType, StateId>::value_type(input, ter));
	else all_next.insert(std::map<InputType, StateId>::value_type(input, ter));
}

bool Automaton::isFin()
{
	for(int i = 0; i < fin_states.size(); i++)
	{
		if(current_state == fin_states[i])
			return true;
	}
	return false;
}

Automaton::State::State(StateId id): id(id){}

StateId Automaton::State::Next(InputType input)
{
	if(!next.count(input))
		return this->Id();
	return next[input];
	/*if(next.count(input))
		return next[input];
	else if(all_next.count[input])
		return all_next[input];
	else next[input];*/
}