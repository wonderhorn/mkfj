#pragma once
#include"utils/Geometry.h"
#include"framework/Physics.h"
#include"framework/MediaAlternative.h"
#include"framework/Task.h"
#include<assert.h>

namespace gfw
{
	class Object;
	template <class T> T* create(Task* t)
	{
		assert(sizeof(Task)+sizeof(T) < Tasklist::MAX_TASK_SIZE);
		new(t->workspace) T();
		T* task = (T*)t->workspace;
		return task;
	}
	template <class T> T* generate(Tasklist& tl)
	{
		auto t = tl.generate();
		return create<T>(t);
	}

	void writeName(const char* src, char dst[32]);
	Object* getObject(Task* t);
	class Object
	{
	public:
		Object();
		virtual ~Object();
		//void create(Task* t);
		virtual void initialized(){ id = total_num_obj++; }
		virtual void run(){}
		virtual void render(gfw::BaseMedia& media){}
		static void* operator new(size_t t, void* ptr);
		static void operator delete(void* ptr, void* ptr2);
		static void operator delete(void* ptr);
		phsc::Physics& Phys(){ return phys; }
		std::string Name()const{ return name; }

		bool alive;
		bool isMover;
		static gfw::Tasklist* tl;

		class Pipe
		{
		public:
			Pipe() : index(-1), id(-1){};
			Pipe(Object* obj, const Tasklist& tl)
			{
				if (obj == nullptr)
				{
					id = -1;
				}
				else
				{
					id = obj->id;
					index = tl.ptr2Index(obj);
				}
			}
			void init(Object* obj, const Tasklist& tl)
			{
				if (obj == nullptr)
				{
					id = -1;
				}
				else
				{
					id = obj->id;
					index = tl.ptr2Index(obj);
				}
			}
			int index;
			int id;
			Object* Dst(Tasklist& tl)const
			{
				if (id < 0)
					return nullptr;
				Object* obj = getObject(tl.NthTask(index));
				if (obj->id == id)
					return obj;
				else return NULL;
			}
		};

	protected:
		char name[32];
		phsc::Physics phys;
		int id;
		static int total_num_obj;
	};

	class TestObject : public Object
	{
	public:
		TestObject();
		virtual void run();
		virtual void render(gfw::BaseMedia& media);
		int age;
	};
};