#include"framework/Task.h"
#include<assert.h>
using namespace gfw;

Tasklist::Tasklist() : num_free(0), num_active(0), head_free((Task*)(buf + MAX_TASK_SIZE)), head_active((Task*)(buf))
{}

Tasklist::iterator Tasklist::begin()
{
	iterator i;
	i.i = head_active->next;
	i.tl = this;
	return i;
}

Tasklist::iterator Tasklist::end()
{
	iterator i;
	i.tl = this;
	i.i = INDEX_ACTIVE;
	return i;
}

Tasklist::iterator Tasklist::remove(iterator i)
{
	assert(i->isActive == true);
	assert(num_active > 0);
	int rtn = i->next;

	i->isActive = false;

	NthTask(i->next)->prev = i->prev;
	NthTask(i->prev)->next = i->next;

	i->next = INDEX_FREE;
	i->prev = head_free->prev;

	head_free->prev = NthTask(head_free->prev)->next = i.i;

	num_active--;
	num_free++;

	i.i = rtn;
	return i;
}

bool Tasklist::initialize()
{
	num_free = MAX_NUM_TASK;
	num_active = 0;

	head_active->next = head_active->prev = INDEX_ACTIVE;
	head_active->tl = this;
	head_active->isActive = true;
	head_active->id = -1;

	head_free->next = 2;
	head_free->prev = MAX_NUM_TASK - 1;
	head_free->tl = this;
	head_free->isActive = false;
	head_free->id = -1;

	for (int i = 2; i < MAX_NUM_TASK; i++)
	{
		Task* t = this->NthTask(i);
		t->next = i + 1;
		t->prev = i - 1;
		t->tl = this;
		t->isActive = false;
		t->id = i;
	}
	this->NthTask(2)->prev = this->NthTask(MAX_NUM_TASK - 1)->next = INDEX_FREE;

	return true;
}

void Tasklist::clone(Tasklist* dst, void(*reset_obj)(void* ptr))const
{
	for (int i = 0; i < BUF_SIZE; i++)
	{
		dst->buf[i] = this->buf[i];
	}
	dst->reset(reset_obj);
}

bool Tasklist::reset(void(*reset_obj)(void* ptr))
{
	num_active = num_free = 0;
	for (int i = 0; i < MAX_NUM_TASK; i++)
	{
		Task* t = this->NthTask(i);
		t->tl = this;
		if (i >= 2)
		{
			if (!t->isActive)
				num_free++;
			else num_active++;

			reset_obj(t->workspace);
		}
	}
	return true;
}

int Tasklist::ptr2Index(void* ptr)const
{
	return ((char*)ptr - buf) / MAX_TASK_SIZE;
}

Tasklist::iterator Tasklist::generate()
{
	assert(num_free > 0);

	int pickedup = head_free->next;
	Task* haprv = NthTask(head_active->prev);
	head_free->next = NthTask(pickedup)->next;
	NthTask(head_free->next)->prev = INDEX_FREE;

	NthTask(pickedup)->next = INDEX_ACTIVE;
	NthTask(pickedup)->prev = head_active->prev;

	haprv->next = pickedup;
	head_active->prev = pickedup;

	num_free--;
	num_active++;

	iterator i;
	i.tl = this;
	i.i = pickedup;
	i->isActive = true;
	return i;
}

Task* Tasklist::NthTask(int i)const
{
	assert(i >= 0 && i < MAX_NUM_TASK);
	return (Task*)(buf + MAX_TASK_SIZE * i);
}


Tasklist::iterator& Tasklist::iterator::operator++()
{
	iterator i;
	i.tl = this->tl;
	i.i = tl->NthTask(this->i)->next;
	return i;

}
Tasklist::iterator Tasklist::iterator::operator++(int)
{
	//i.tl = this->tl;
	i = tl->NthTask(this->i)->next;
	return *this;
}
bool Tasklist::iterator::operator == (const iterator& iter)const
{
	return (this->tl == iter.tl && this->i == iter.i);
}
Tasklist::iterator::operator Task* ()
{
	return tl->NthTask(this->i);
}