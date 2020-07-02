#pragma once
#include<vector>
#include<iostream>
#include<fstream>
#include<cstdlib>
namespace space_tree
{
	//using namespace std;
	template<class T>
	class tree
	{
	public:
		class iterator;
	private:
		class node
		{
			friend class tree;
			friend class tree::iterator;
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
		void printFile(std::string filename)const;
		bool load(const std::string& filename);
		bool parse(const std::string& src);
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
			int ChildNum();
			T operator *();
			T* operator ->();
			bool operator ==(const iterator& obj)const;
			bool operator !=(const iterator& obj)const;
			bool operator >(const iterator& obj)const;
			bool operator <(const iterator& obj)const;
		private:
			iterator();
			iterator(node* n);
			node* n;
			int next_child;
		};

		iterator Root();
		iterator push_back(iterator it , const T& t);

	private:
		node* current_node;
		node root;
		int current_depth;

	public:
		T& Value(){return current_node->value;}
		node* goToRoot();
		node* goToParent();
		node* searchChild();
		int ChildNum()const{return current_node->children.size();}
		T Child(int i)const{return current_node->children[i]->value;}
		node* goToChild(int i);
		int ID_currentNode(){return current_node->id;}
		void joint(const tree<T>& t, int index);
		void joint(const node& n, int index);
		std::ostream& operator <<(std::ostream& ost)const;
	};
};

#include"tree/tree_interator.h"

#include"tree/tree_tree.h"

#include"tree/tree_node.h"