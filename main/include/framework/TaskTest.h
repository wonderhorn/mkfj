#include"framework/Screen.h"
#include"framework/Object.h"

class TaskTest : public gfw::Screen
{
public:
	TaskTest() : age(0)
	{
		using namespace gfw;
		tl.initialize();
		generate<TestObject>(tl);
	}
	bool initialize(){ return true; }
	void finalize(){}
	eScreenState run(gfw::BaseMedia& media)
	{
		using namespace gfw;
		using namespace gmtr;
		if (age % 15 == 0)
		{
			TestObject* to = generate<TestObject>(tl);
			to->Phys().p = Vector2D(0, 20.0 * ((age / 15) % 24));
		}

		for (auto t = tl.begin(); t != tl.end();)
		{
			Object* obj = getObject(t);
			obj->run();

			if (obj->alive)
				t++;
			else
			{
				delete (Object*)t->workspace;
				t = tl.remove(t);
			}
		}
		age++;
		return eScreenState::Continuing;
	}
	void render(gfw::BaseMedia& media)
	{
		using namespace gfw;
		for (auto t = tl.begin(); t != tl.end(); t++)
		{
			Object* obj = getObject(t);
			obj->render(media);
		}
	}

private:
	gfw::Tasklist tl;
	int age;
};