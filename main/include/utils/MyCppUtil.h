#pragma once
#include<functional>
#include<vector>
#include<strstream>
#include<stdlib.h>
//#define mymap<_container, T_src, T_dst> map<_container<T_src>, T_src, _container<T_dst>, T_dst>

//#define $P std::pair
//#define $V std::vector
#define $cref(t) const t&

#define $for_i(lim) for(int (i) = 0; (i) < (lim); (i)++)
#define $for_j(lim) for(int (j) = 0; (j) < (lim); (j)++)
#define $for_k(lim) for(int (k) = 0; (k) < (lim); (k)++)

template <class T> class VectorInserter
{
public:
	VectorInserter(std::vector<T>& v) : v(v){}
	std::vector<T>& v;
	operator std::vector<T>&(){ return v; }
	VectorInserter& operator , (const T& t)
	{
		//std::cout << v.size() << std::endl;
		v.push_back(t);
		return *this;
	}
};
template <class T> VectorInserter<T> operator << (std::vector<T>& v, T val)
{
	v.push_back(val);
	return VectorInserter<T>(v);
}

namespace myutil
{
	template <class T> std::string ToStr(const T& t)
	{
		std::strstream ss;
		ss << t << '\0';
		return ss.str();
	}

	template <class container_src> container_src& shuffle(container_src& src)
	{
		$for_k(src.size() * 100)
		{
			int n1 = rand() % (src.size() - 1);
			int n2 = rand() % (src.size() - 1);
			if (n1 != n2)
			{
				auto temp = src[n1];
				src[n1] = src[n2];
				src[n2] = temp;
			}

		}
		return src;
	}

	template <class container_src> container_src slice(const container_src& src, int ori, int ter)
	{
		container_src rtn;
		int counter = 0;
		for (auto i = src.begin(); i != src.end(); i++)
		{
			if (counter >= ori && (counter < ter || ter < 0))
				rtn.push_back(*i);
			counter++;
		}
		return rtn;
	}

	template <class _Dst, class _Container, class _Func> _Dst & map(_Func f, const _Container& src, _Container& dst)
	{
		dst.clear();
		for (auto i = src.begin(); i != src.end(); i++)
		{
			dst.push_back(f(*i));
		}
		return dst;
	}
	template <class _Dst, class _Container, class _Func> _Dst map(_Func f, const _Container& src)
	{
		_Dst dst;
		for (auto i = src.begin(); i != src.end(); i++)
		{
			dst.push_back(f(*i));
		}
		return dst;
	}

	template <class _Container, class _Func> _Container & transform(_Func f, _Container& src)
	{
		for (auto i = src.begin(); i != src.end(); i++)
		{
			*i = f(*i);
		}
		return src;
	}

	template <class _Container, class _Func> _Container & filter(_Func f, const _Container& src, _Container& dst)
	{
		dst.clear();
		for (auto i = src.begin(); i != src.end(); i++)
		{
			if (f(*i))
				dst.push_back(*i);
		}
		return dst;
	}
	template <class _Container, class _Func> _Container filter(_Func f, const _Container& src)
	{
		_Container dst;
		for (auto i = src.begin(); i != src.end(); i++)
		{
			if (f(*i))
				dst.push_back(*i);
		}
		return dst;
	}

	template <class _Dst, class _Container, class _Func> _Dst reduce(_Func f, const _Container& src)
	{
		_Dst rtn(0);
		for (auto i = src.begin(); i != src.end(); i++)
		{
			rtn = f(rtn, *i);
		}
		return rtn;
	}

	template <class _Dst, class _Container> _Dst sum(const _Container& src)
	{
		return reduce<_Dst, _Container>([](const _Dst& d1, const _Dst& d2){return d1 + d2; }, src);
	}

	/*template <class _Dst, class _Container> _Dst max(const _Container& src)
	{
		return reduce<_Dst, _Container>([](const _Dst& d1, const _Dst& d2)->bool{return d1 > d2 ? d1 : d2; }, src);
	}

	template <class _Dst, class _Container> _Dst min(const _Container& src)
	{
		return reduce<_Dst, _Container>([](const _Dst& d1, const _Dst& d2)->bool{return d1 < d2 ? d1 : d2}, src);
	}
	template <class _Container> int argmax(const _Container& src)
	{
		int rtn = 0;
		for (auto i = 0; i != src.size(); i++)
		{
			if (src[i] > src[rtn])
				rtn = i;
		}
		return rtn;
	}
	template <class _Container> int argmin(const _Container& src)
	{
		int rtn = 0;
		for (auto i = 0; i != src.size(); i++)
		{
			if (src[i] < src[rtn])
				rtn = i;
		}
		return rtn;
	}*/

};