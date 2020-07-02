#include<iostream>
#include<sstream>

namespace space_tree
{
	template<class T> tree<T>::node::node(): parent(NULL), id(-1){}

	template<class T> tree<T>::node::node(node* parent, const T& t): parent(parent), value(t), id(-1)
	{}

	template<class T> tree<T>::node::~node()
	{
		/*for(int i = 0; i < children.size(); i++)
		{
			node* deletee = children[i];
			delete deletee;
		}*/
	}

	template<class T> typename tree<T>::node *tree<T>::node::addChild(const T &t)
	{
		node* n = new node(this, t);
		n->id = children.size();
		children.push_back(n);
		return n;
	}

	template<class T> typename tree<T>::node *tree<T>::node::addChild(const T &t, int index)
	{
		if(index >= children.size())
		{
			return addChild(t);
			//throw "exception: index out of range";
		}

		node* n = new node(this, t);
		n->id = index;

		int j = 0;
		for(typename std::vector<node*>::iterator i = children.begin(); i != children.end(); i++, j++)
		{
			if(j == index)
			{
				children.insert(i, n);
				break;
			}
			/*else
			{
				if(j > index)
					(*i)->id++;
				i++;
			}*/
		}

		j = 0;
		for(typename std::vector<node*>::iterator i = children.begin(); i != children.end(); i++, j++)
		{
			if(j > index)
				(*i)->id++;
		}

		//children.push_back(n);
		return n;
	}

	template<class T> typename tree<T>::node *tree<T>::node::clone()
	{
		node* n = new node();
		n->value = this->value;
		n->id = this->id;
		for(int i = 0; i < children.size(); i++)
		{
			node* child = children[i]->clone();
			n->children.push_back(child);
			child->parent = n;
		}
		return n;
	}

	template<class T> void tree<T>::node::print(std::ostream& os, int depth)const
	{
		using namespace std;
		for(int i = 0; i < depth; i++)
			os << "\t";
		//os << value << std::endl;
		typename std::vector<tree<T>::node*>::const_iterator it = children.begin();
		while(it != children.end())
		{
			(*it)->print(os, depth + 1);
			it++;
		}
	}

	template<class T> void tree<T>::node::count(int& n)
	{
		n++;
		typename std::vector<tree<T>::node*>::const_iterator it = children.begin();
		while(it != children.end())
		{
			(*it)->count(n);
			it++;
		}
	}

	template<class T> typename tree<T>::node* tree<T>::node::pickup(int& i)
	{
		if(i == 0)
			return this;

		i--;

		typename std::vector<tree<T>::node*>::const_iterator it = children.begin();
		while(it != children.end())
		{
			node* n = (*it)->pickup(i);
			if(n)
				return n;
			it++;
		}

		return NULL;
	}
};