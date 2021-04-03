﻿#pragma once
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <windows.h>
#include <cctype>
#include <string>

using namespace std;

namespace binTree_modul
{

	//-------------------------------------
	typedef char base;

	struct node {
		base info;         //элемент
		node* lt;         //указатель на левую ветку
		node* rt;         //указатель на правую ветку
		node() { 
			info = NULL;
			lt = NULL; 
			rt = NULL; 
		}
	};

	typedef node* binTree; // "представитель" бинарного дерева

	binTree Create(void);
	base RootBT(binTree); // функция, возвращающая корень дерева
	binTree Left(binTree);// функция, возвращающая левую ветку дерева
	binTree Right(binTree);// функция, возвращающая правую ветку дерева
	binTree ConsBT(const base& x, binTree& lst, binTree& rst);  //функция сборки бинарного дерева
	void destroy(binTree&);   //удаление бинарного дерева
	
} // end of namespace binTree_modul

namespace binTree_modul
{

	//-------------------------------------
	binTree Create()
	{
		return NULL;
	}
	//-------------------------------------
	base RootBT(binTree b)			// для непустого бин.дерева
	{
		if (b == NULL) { cerr << "Error: RootBT(null) \n"; exit(1); }
		else return b->info;
	}
	//-------------------------------------	
	binTree Left(binTree b)		// для непустого бин.дерева
	{
		if (b == NULL) { cerr << "Error: Left(null) \n"; exit(1); }
		else return b->lt;
	}
	//-------------------------------------	
	binTree Right(binTree b)		// для непустого бин.дерева
	{
		if (b == NULL) { cerr << "Error: Right(null) \n"; exit(1); }
		else return b->rt;
	}
	//-------------------------------------		
	binTree ConsBT(const base & x, binTree & lst, binTree & rst)
	{
		binTree p;
		p = new node;
		if (p != NULL) {
			p->info = x;
			p->lt = lst;
			p->rt = rst;
			return p;
		}
		else { cerr << "Memory not enough\n"; exit(1); }
	}
	//-------------------------------------			
	void destroy(binTree & b)
	{
		if (b != NULL) {
			destroy(b->lt);
			destroy(b->rt);
			delete b;
			b = NULL;
		}
	}

} // end of namespace h_list