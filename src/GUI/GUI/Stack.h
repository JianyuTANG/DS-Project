#pragma once
#include "stdio.h"
#include "stdlib.h"

//��������ʵ�ֵ�ջ

template <class T>
struct linknode          //����ڵ�
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
	void push(T x);       //����һ��Ԫ��
	bool pop();           //��ջ��ȡ��һ��Ԫ�� 
	bool empty();         //��ѯջ�Ƿ�Ϊ�գ���Ϊ�շ���0�����򷵻�1
	T elemOnTheTop();     //����ջ����Ԫ��
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