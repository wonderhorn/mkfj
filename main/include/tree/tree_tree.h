#include<vector>

#ifndef GFW_WIN32
#define strncpy_s(a, b, c, d) strncpy((a), (c), (d))
#endif

namespace space_tree
{
	template<class T> tree<T>::tree(): root(), current_node(&root), current_depth(0)
	{}

	template<class T> tree<T>::tree(const tree& obj): root(), current_node(&root), current_depth(0)
	{
		//root = obj.root;
		for(int i = 0; i < obj.root.children.size(); i++)
		{
			node* child = obj.root.children[i]->clone();
			root.children.push_back(child);
			child->parent = &root;
		}
	}

	template<class T> tree<T> tree<T>::operator = (const tree<T>& obj)
	{
		for(int i = 0; i < root.children.size(); i++)
		{
			delete root.children[i];
		}
		root.children.clear();

		for(int i = 0; i < obj.root.children.size(); i++)
		{
			node* child = obj.root.children[i]->clone();
			root.children.push_back(child);
			child->parent = &root;
		}

		return *this;
	}

	template<class T> tree<T>::~tree()
	{
		/*for(int i = 0; i < root.children.size(); i++)
		{
			delete root.children[i];
		}*/
	}

	template<class T> void tree<T>::push_back(const T& t)
	{
		current_node = current_node->addChild(t);
		current_depth++;
	}

	template<class T> typename tree<T>::iterator tree<T>::push_back(iterator it, const T& t)
	{
		return iterator(it.n->addChild(t));
	}

	template<class T> void tree<T>::insert(const T& t, int i)
	{
		current_node = current_node->addChild(t, i);
		current_depth++;
	}

	template<class T> void tree<T>::joint(const node& n, int index)
	{
		//if(1)
		if(index < 0)
			push_back(n.value);
		else insert(n.value, index);
		for(int i = 0; i < n.children.size(); i++)
		{
			joint(*n.children[i], -1);
		}
		goToParent();
	}

	template<class T> void tree<T>::joint(const tree<T>& t, int index)
	{
		node* n_prev = current_node;
		for(int i = 0; i < t.root.children.size(); i++)
		{
			//t.root.children[i]->id = index;
			joint(*t.root.children[i], index);

		}
		node* n_now = current_node;

	}

	/*template<class T> void tree<T>::joint(const tree<T>& t)
	{
		for(int i = 0; i < t.root.children.size(); i++)
		{
			joint(*(t.root.children[i]));
		}
	}*/
	template<class T> typename tree<T>::iterator tree<T>::Root()
	{
		return iterator(&root);
	}

	template<class T> typename tree<T>::node* tree<T>::goToRoot()
	{
		return (current_node = &root);
	}

	template<class T> typename tree<T>::node* tree<T>::goToParent()
	{
		if(!current_node)
			return NULL;
		else
		{
			current_depth--;
			assert(current_node->parent != NULL);
			return (current_node = current_node->parent);
		}
	}

	template<class T> typename tree<T>::node* tree<T>::goToChild(int i)
	{
		return(current_node = current_node->children[i]);
	}


	template<class T> typename tree<T>::node* tree<T>::searchChild()
	{
		if(!current_node)
			return NULL;
		current_depth++;
	}

	template<class T> void tree<T>::print()const
	{
		root.print(std::cout, 0);
	}

	template<class T> void tree<T>::printFile(std::string filename)const
	{
		std::ofstream ofs(filename.c_str());
		for(int i = 0; i < root.children.size(); i++)
		{
			root.children[i]->print(ofs, 0);
		}
	}

	template<class T> bool tree<T>::load(const std::string& filename)
	{
		std::ifstream ifs(filename.c_str());
		if(ifs.fail())
			return false;

		char buf[10240] = {};
		int depth_prev = -1;
		while(!ifs.eof())
		{
			ifs.getline(buf, 1024);

			int depth = 0;
			char* s = buf;
			while(*s == '\t')
			{
				depth++;
				s++;
			}
			if(*s == '\0')
				continue;
			depth;
			for(int i = 0; i <= depth_prev - depth; i++)
				goToParent();
			push_back(T(s));
			depth_prev = depth;
		}

		return true;
	}

	template<class T> bool tree<T>::parse(const std::string& src)
	{
		int cursor = 0;
		int prev = 0;
		//char buf[1024] = {};
		int depth_prev = -1;
		while (cursor < src.size())
		{
			while (src[cursor] != '\n' && cursor < src.size())
			{
				cursor++;
			}
			/*if (cursor >= src.size())
				break;*/
			char buf[10240] = {};
			strncpy_s(buf, 10240
				, &src[prev], cursor - prev);
			int depth = 0;
			char* s = buf;
			while (*s == '\t')
			{
				depth++;
				s++;
			}
			if (*s == '\0')
				continue;
			for (int i = 0; i <= depth_prev - depth; i++)
				goToParent();
			push_back(T(s));
			depth_prev = depth;

			prev = cursor;
		}

		return true;
	}

	template<class T> int tree<T>::countNodes()
	{
		int rtn = 0;
		current_node->count(rtn);
		return rtn;
	}

	template<class T> void tree<T>::goToNode(int i)
	{
		current_node = current_node->pickup(i);
	}

	template<class T> void tree<T>::deleteCurrent()
	{
		node* deletee = current_node;
		goToParent();

		typename std::vector<node*>::iterator it = current_node->children.begin();
		while(it != current_node->children.end())
		{
			if(*it == deletee)
			{
				it = current_node->children.erase(it);
			}
			else it++;
		}

		for(int i = 0; i < current_node->children.size(); i++)
			current_node->children[i]->id = i;

		delete deletee;
	}

	//template<class T> void tree<T>::deleteChildren();

	template<class T> tree<T> tree<T>::subtree()
	{
		tree<T> t;
		node* n = current_node->clone();
		t.root.children.push_back(n);
		n->parent = &t.root;
		/*for(int i = 0; i < this->root.children.size(); i++)
		{
			node* child = this->root.children[i]->clone();
			t.root.children.push_back(child);
			child->parent = &t.root;
		}*/
		return t;
	}
}