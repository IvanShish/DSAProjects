#include <iostream>
#include <fstream>
#include <Windows.h>
#include <stdlib.h>
#include <string>

#pragma once

template <class Item>
class STACK {
private:
	Item* s;       //������� � �����
	int size;      //������ ����� �����
	int top;       //������ �������� �������� �����
public:
	STACK(int = 100);
	~STACK();

	inline void push(Item);       //������� ���������� �������� � ����
	inline Item pop();                //������� �������� �������� �� �����
	inline void printStack();         //������� ������ �����
	inline const Item& peek(int) const;    //�������, ������������ ������� ������� �����
	inline int getTop() const;        //�������, ������������ ������ �������� �������� �����
};

template <class Item>
STACK<Item>::STACK(int currSize): size(currSize){
	s = new Item[size];

	if (s == NULL) {
		delete []s;
		exit(0);
	}
	top = 0;
}

template <class Item>
STACK<Item>::~STACK() {
	delete []s;
}

template <class Item>
inline void STACK<Item>::push(Item elem) {
	for (int i = 0; i < top; i++) std::cout << " ";
	std::cout << "push for element " << elem << std::endl;
	s[top++] = elem;
}

template <class Item>
inline Item STACK<Item>::pop() {
	for (int i = 0; i < top - 1; i++) std::cout << " ";
	std::cout << "pop for element " << s[top-1] << std::endl;
	return s[--top];
}

template <class Item>
inline void STACK<Item>::printStack() {
	for (int i = top - 1; i >= 0; i--) {
		std::cout << s[i] << std::endl;
	}
}

template <class Item>
inline int STACK<Item>::getTop() const {
	return top;
}

template <class Item>
inline const Item& STACK<Item>::peek(int num) const{
	if (num >= top)
		exit(3);

	return s[top - num - 1]; // ������� n-� ������� �����
}