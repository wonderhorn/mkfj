#pragma once
#include<limits.h>
#include<float.h>
namespace gmtr
{
	class Parameter
	{
	public:
		const static double eps;
	};

	class Vector2D
	{
	public:
		Vector2D(){}
		Vector2D(double x, double y);
		double x, y;

		double l2()const;
		double sqsum()const;
		double theta()const;
		Vector2D regularized()const;
		Vector2D operator +(const Vector2D& v)const;
		Vector2D operator -(const Vector2D& v)const;
		Vector2D operator -()const;
		double operator *(const Vector2D& v)const;
		Vector2D operator *(double d)const;
		Vector2D operator /(double d)const;
		Vector2D& operator +=(const Vector2D& v);
		Vector2D& operator -=(const Vector2D& v);
		Vector2D& operator *=(double d);

		static Vector2D polar(double r, double theta);
	};
	Vector2D operator *(double d, const Vector2D);

	class Geometry
	{
	public:
		Geometry(){}
		virtual bool includes(const Vector2D& v)const = 0;
		virtual double distance(const Vector2D& v)const = 0;
		//virtual std::vector<Vector2D> RepresentingPoints()const = 0;
	};
	class NullGeometry : public Geometry
	{
	public:
		NullGeometry(){}
		bool includes(const Vector2D& v)const{ return false; }
		double distance(const Vector2D& v)const{ return FLT_MAX; }
		static NullGeometry instance;
	};

	class Sphere : public Geometry
	{
	public:
		Sphere(gmtr::Vector2D c, double r);
		Vector2D c;
		double r;
		bool includes(const Vector2D& v)const;
		double distance(const Vector2D& v)const;
	};

	class Line
	{
	public:
		Line(const Vector2D& ori, const Vector2D& ter);
		Vector2D ori, ter;
		double distance(const Vector2D& v)const;
		bool crossing(const Line& l)const;
	};

	class Rectangle: public Geometry
	{
	public:
		Rectangle(gmtr::Vector2D lefttop, gmtr::Vector2D rightbottom);
		Rectangle(int x1, int y1, int x2, int y2);
		Rectangle(){}
		int x1, x2, y1, y2;
		bool includes(const Vector2D& v)const;
		double distance(const Vector2D& v)const;
	};
};