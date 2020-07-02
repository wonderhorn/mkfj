#pragma once
#include<map>
#include<vector>
namespace myutil
{
	template <class _Src, class _Dst> class Histogram
	{
	public:
		Histogram(){}
		_Dst& operator[](const _Src& src)
		{
			if (m.count(src))
				return m[src];
			else
			{
				m.insert(typename std::map<_Src, _Dst>::value_type(src, 0));
				return m[src];
			}
		}
		std::vector<_Src> keyvec()
		{
			std::vector<_Src> rtn(m.size());
			int counter = 0;
			for (auto kv : m)
			{
				rtn[counter] = kv.first;
				counter++;
			}
			return rtn;
		}
		std::vector<_Src> valvec()
		{
			std::vector<_Src> rtn(m.size());
			int counter = 0;
			for (auto kv : m)
			{
				rtn[counter] = kv.second;
				counter++;
			}
			return rtn;
		}
	private:
		std::map<_Src, _Dst> m;
	};
};