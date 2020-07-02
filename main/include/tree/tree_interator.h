namespace space_tree
{
	template<class T> tree<T>::iterator::iterator(): n(NULL), next_child(0){}

	template<class T> tree<T>::iterator::iterator(node* n): n(n), next_child(0){}

	template<class T> int tree<T>::iterator::ChildNum()
	{
		return n->children.size();
	}

	template<class T> bool tree<T>::iterator::goToParent()
	{
		if(n->parent)
		{
			n = n->parent;
			return true;
		}
		return false;
	}

	template<class T> bool tree<T>::iterator::goToChild(int i)
	{
		if(n->children.size() > i)
		{
			n = n->children[i];
			return true;
		}
		return false;
	}

	template<class T> T tree<T>::iterator::operator *()
	{
		return n->value;
	}

	template<class T> T* tree<T>::iterator::operator ->()
	{
		return &n->value;
	}

	template<class T> bool tree<T>::iterator::operator ==(const iterator& obj)const
	{
		return this->n == obj.n;
	}

	template<class T> bool tree<T>::iterator::operator !=(const iterator& obj)const
	{
		return this->n != obj.n;
	}

	template<class T> bool tree<T>::iterator::operator >(const iterator& obj)const
	{
		return this->n > obj.n;
	}

	template<class T> bool tree<T>::iterator::operator <(const iterator& obj)const
	{
		return this->n < obj.n;
	}
}