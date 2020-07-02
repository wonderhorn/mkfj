#include"framework/Object.h"

using namespace gfw;
using namespace gmtr;
gfw::Tasklist* Object::tl;
int gfw::Object::total_num_obj = 0;

void gfw::writeName(const char* src, char dst[32])
{
	//strncpy(dst, src, 32);
	for (int i = 0; i < 32; i++)
	{
		dst[i] = 0;
	}
	for (int i = 0; i < strlen(src); i++)
	{
		dst[i] = src[i];
	}
}

Object* gfw::getObject(Task* t)
{
	return (Object*)t->workspace;
}

Object::Object() : alive(true)
{
	writeName("object", name);
}
Object::~Object(){}

void* Object::operator new(size_t t, void* buf){return buf;}
void Object::operator delete(void* ptr, void* ptr2){}
void Object::operator delete(void* ptr){}

TestObject::TestObject() : age(0)
{
	writeName("testobject", name);
}
void TestObject::run()
{
	if (age >= 45)
		alive = false;
	age++;
}
void TestObject::render(gfw::BaseMedia& media)
{
	BaseGraphics* graphics = media.getGraphics();
	graphics->drawString(name, phys.p.x, phys.p.y, 16 * 32, 18);
}