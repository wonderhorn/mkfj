#pragma once

namespace gfw
{
	class Object;
	class Task;
	//void resetObject(const char* name, void* ptr);
	class Tasklist
	{
		friend class Object;
	public:
		Tasklist();
		bool initialize();
		bool reset(void(*reset_obj)(void* ptr));
		void clone(Tasklist* dst, void(*reset_obj)(void* ptr))const;
		int ptr2Index(void* ptr)const;
		char* Buffer() { return buf; }
		static const int MAX_TASK_SIZE = 2048, MAX_NUM_TASK = 512;
		static const int BUF_SIZE = MAX_TASK_SIZE * (MAX_NUM_TASK + 2);

		class iterator
		{
			friend class Tasklist;
		public:
			iterator& operator++();
			iterator operator++(int);
			bool operator == (const iterator& iter)const;
			bool operator != (const iterator& iter)const{ return !(*this == iter); }
			operator Task*();
			Task* operator ->(){ return (Task*)*this; }
		private:
			iterator(){}
			int i;
			Tasklist* tl;
		};

		iterator begin();
		iterator end();
		iterator generate();
		iterator remove(iterator i);

	private:
		Task* NthTask(int i)const;
		char buf[BUF_SIZE];
		int num_free, num_active;
		Task* head_free, *head_active;
		const int INDEX_ACTIVE = 0, INDEX_FREE = 1;
	};
	class Task
	{
		friend class Tasklist;
	private:
		Task(){}
		Tasklist* tl;
		int id;
		int next, prev;
		bool isActive;
	public:
		char workspace[1];
	};
};