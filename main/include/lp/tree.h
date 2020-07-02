#pragma once
#include<vector>
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<iostream>
#include<ostream>
#include<strstream>

namespace space_tree
{
	//using namespace std;
	template<class T>
	class tree
	{
		class node
		{
			friend class tree;
		public:
			node(node* parent, const T& t);
			node();
			~node();
			//node(const node& obj);
			node* addChild(const T& t);
			node* addChild(const T& t, int index);
			void print(std::ostream& os, int depth)const;
			node* clone();
		private:
			//node(const node& obj){}
			T value;
			int id;
			node* parent;
			std::vector<node*> children;
			void count(int& n);
			node* pickup(int& i);
		};
	public:
		tree();
		tree(const tree& obj);
		tree<T> operator = (const tree<T>& obj);
		~tree();
		void push_back(const T& t);
		void insert(const T& t, int i);
		//void push_front(const T& t);

		void print()const;
		void printFile(const std::string& filename)const;
		bool load(const std::string& filename);
		static tree<T> parse(const std::string& src);
		int countNodes();
		void goToNode(int i);
		void deleteCurrent();
		tree<T> subtree();

		class iterator
		{
			friend class tree;
		public:
			bool goToParent();
			bool goToChild(int i);
			iterator getChild(int i);
			int ChildNum();
			T& operator *();
			T* operator ->();
			bool operator ==(const iterator& obj)const;
			bool operator !=(const iterator& obj)const;
			bool operator >(const iterator& obj)const;
			bool operator <(const iterator& obj)const;
			void set(const T& t){n->value = t;}
		private:
			iterator();
			iterator(node* n);
			node* n;
			int next_child;
		};
		friend class tree::iterator;

		iterator Root();
		iterator push_back(iterator it , const T& t);
		iterator insert(iterator it , const T& t, int i);
		tree<T> subtree(iterator it);

	private:
		node* current_node;
		node root;
		int current_depth;

	public:
		T Value(){return current_node->value;}
		node* goToRoot();
		node* goToParent();
		node* searchChild();
		int ChildNum()const{return current_node->children.size();}
		T Child(int i)const{return current_node->children[i]->value;}
		node* goToChild(int i);
		int ID_currentNode(){return current_node->id;}
		void joint(const tree<T>& t, int index);
		iterator joint(iterator it, const tree<T>& t, int index);
		void joint(const node& n, int index);
		void joint(iterator it, const node& n, int index);
		iterator deleteCurrent(iterator it);

		std::ostream& operator <<(std::ostream& ost)const;
	};
	template <class T> std::vector<typename tree<T>::iterator> linearize(tree<T>& t, bool depth_first);
};

#include"tree/tree_interator.h"

#include"tree/tree_tree.h"

#include"tree/tree_node.h"