#pragma once
#include<string>
#include<strstream>
#include<stdlib.h>
#include<vector>
#include<time.h>
#include<assert.h>
namespace gfw
{
	std::string num2str(int n);

	class Timer
	{
	public:
		Timer() : limit(-1), tick(0){}
		Timer(int limit): tick(0), limit(limit){}
		void advance(){ tick++; }
		bool alarm(){ return limit >= 0 && tick >= limit; }
		bool moving(){ return  limit >= 0 && tick < limit; }
		void reset(int limit){ tick = 0, this->limit = limit; }
		void reset(){ reset(limit); }
		int rest(){ return limit - tick; }
	private:
		int tick, limit;
	};

	class MyRand
	{
	public:
		MyRand(int seed) : seed(seed), cursor(0), v(max_num_rand)
		{
			srand(seed);
			for (int i = 0; i < max_num_rand; i++)
				v[i] = rand();
		}
		MyRand() : cursor(0), v(max_num_rand)
		{
			srand((unsigned int)time(NULL));
			for (int i = 0; i < max_num_rand; i++)
				v[i] = rand();
		}
		void reset(int seed)
		{
			srand(seed);
			for (int i = 0; i < max_num_rand; i++)
				v[i] = rand();
			cursor = 0;
		}
		int get()
		{
			int rtn = v[cursor++];
			//assert(cursor < max_num_rand);
			cursor = cursor % max_num_rand;
			return rtn;
		}
		int seed;
		int cursor;
		std::vector<int> v;
		static const int max_num_rand = 1e7;
	};
};