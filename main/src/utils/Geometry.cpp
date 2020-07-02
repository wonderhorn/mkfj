#include"utils/Geometry.h"
#define _USE_MATH_DEFINES
#include<cmath>
using namespace gmtr;

#define max(p, q) (p) >= (q) ? (p): (q)
#define min(p, q) (p) >= (q) ? (q): (p)

const double Parameter::eps = 0.005;
Vector2D gmtr::operator *(double d, const Vector2D v)
{
	return Vector2D(v.x * d, v.y * d);
}

Vector2D::Vector2D(double x, double y) : x(x), y(y){}

double Vector2D::l2()const{ return sqrt(x*x + y*y); }
double Vector2D::sqsum()const{ return (x*x + y*y); }
double Vector2D::theta()const
{
	if (x == 0 && y == 0)
		return 0;
	double cos_theta = x / l2();
	if (cos_theta > 1.0)
		cos_theta = 1.0;
	else if (cos_theta < -1.0)
		cos_theta = -1.0;
	double t = acos(cos_theta);
	if (y < 0)
		//t += M_PI;
		t = 2 * M_PI - t;
	return t;
}

Vector2D Vector2D::regularized()const{ return *this / (this->l2() + 0.0001); }
Vector2D Vector2D::operator +(const Vector2D& v)const
{
	return Vector2D(x + v.x, y + v.y);
}
Vector2D Vector2D::operator -(const Vector2D& v)const
{
	return Vector2D(x - v.x, y - v.y);
}
Vector2D Vector2D::operator - ()const
{
	return Vector2D(-x, -y);

}
double Vector2D::operator *(const Vector2D& v)const
{
	return x * v.x + y * v.y;
}
Vector2D Vector2D::operator *(double d)const
{
	return Vector2D(x * d, y * d);
}
Vector2D Vector2D::operator /(double d)const
{
	return Vector2D(x / d, y / d);
}
Vector2D& Vector2D::operator +=(const Vector2D& v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}
Vector2D& Vector2D::operator -=(const Vector2D& v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}
Vector2D& Vector2D::operator *=(double d)
{
	this->x *= d;
	this->y *= d;
	return *this;
}

Vector2D Vector2D::polar(double r, double theta)
{
	return Vector2D(r * cos(theta), r * sin(theta));
}


Sphere::Sphere(gmtr::Vector2D c, double r) : c(c), r(r){}

bool Sphere::includes(const Vector2D& v)const
{
	return (c - v).sqsum() < r*r;
}
double Sphere::distance(const Vector2D& v)const
{
	return max(0, (c - v).l2() - r);
}


Line::Line(const Vector2D& ori, const Vector2D& ter):ori(ori), ter(ter){}
double Line::distance(const Vector2D& v)const
{
	Vector2D dir = ter - ori;
	double theta1 = dir.regularized() * (v - ori);
	double theta2 = -(dir.regularized() * (v - ter));
	if (theta1 >= 0 && theta2 >= 0)
	{
		return (v - (dir.regularized() * theta1 + ori)).l2();
	}
	else if (theta1 < 0)
	{
		return (v - ori).l2();
	}
	else
	{
		return (v - ori).l2();
	}
}
bool Line::crossing(const Line& l)const
{
	return false;
}


Rectangle::Rectangle(gmtr::Vector2D lefttop, gmtr::Vector2D rightbottom)
{
	x1 = lefttop.x;
	y1 = lefttop.y;
	x2 = rightbottom.x;
	y2 = rightbottom.y;
}
Rectangle::Rectangle(int x1, int y1, int x2, int y2): x1(x1), y1(y1), x2(x2), y2(y2){}

bool Rectangle::includes(const Vector2D& v)const
{
	bool result = x1 <= v.x && v.x <= x2 && y1 <= v.y && v.y <= y2;
	return result;
}
double Rectangle::distance(const Vector2D& v)const
{
	double d1 = Line(Vector2D(x1, y1), Vector2D(x2, y1)).distance(v);
	double d2 = Line(Vector2D(x2, y1), Vector2D(x2, y2)).distance(v);
	double d3 = Line(Vector2D(x2, y2), Vector2D(x1, y2)).distance(v);
	double d4 = Line(Vector2D(x1, y2), Vector2D(x1, y1)).distance(v);
	return min(min(d1, d2), min(d3, d4));
}