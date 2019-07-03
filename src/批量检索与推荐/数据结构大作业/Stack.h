#pragma once
#include "stdio.h"
#include "stdlib.h"

//基于链表实现的栈

template <class T>
struct linknode          //链表节点
{
	T data;
	linknode *next;
	linknode *pre;
};

template <class T>
class Stack
{
public:
	Stack();
	~Stack();
	void push(T x);       //加入一个元素
	bool pop();           //从栈顶取出一个元素 
	bool empty();         //查询栈是否为空，如为空返回0，否则返回1
	T elemOnTheTop();     //访问栈顶的元素
private:
	linknode<T> *head;
	linknode<T> *tail;
	int length;
};

template <class T>
Stack<T>::Stack()
{
	length=0;
	head=NULL;
	tail=NULL;
}

template <class T>
Stack<T>::~Stack()
{

}

template <class T>
void Stack<T>::push(T x)
{
	linknode<T> *temp;
	temp=new linknode<T>;
	temp->data=x;
	temp->next=NULL;
	temp->pre=NULL;
	if(length==0)
	{
		head=temp;
		tail=temp;
		length=1;
	}
	else
	{
		tail->next=temp;
		temp->pre=tail;
		tail=temp;
		length++;
	}
}

template <class T>
bool Stack<T>::pop()
{
	if(length==0)
		return 0;
	if(length==1)
	{
		delete tail;
		tail=NULL;
		head=NULL;
		length=0;
	}
	else
	{
		linknode<T> *temp;
		temp=tail->pre;
		delete tail;
		temp->next=NULL;
		tail=temp;
		length--;
	}
	return 1;
}

template <class T>
bool Stack<T>::empty()
{
	if(length==0)
		return 0;
	else
		return 1;
}

template <class T>
T Stack<T>::elemOnTheTop()
{
	if(length==0)
		;
	else
		return tail->data;
}