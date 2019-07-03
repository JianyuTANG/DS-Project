#pragma once
#include <cstdio>
#include <cstdlib>
#include "CharString.h"


struct linknode      //�ĵ�����ڵ�
{
	int filecode;    //�ļ���ţ��ļ�����
	int num;         //���ִ���
	int occurnum;    //�����������ҳ�еĲ�ͬ�ؼ������� ���ڽ���ͬ�ؼ��ʵ��ĵ�����ϲ�ʱ���ؼ����������� ���������ĵ�ʱ��ֵΪ1����
	linknode *next,*pre;
};

class FileLinkList
{
public:
	CharString key;
	linknode *head;
	int doc_frequency;
	void Add(int code);
	void Add_for_merge(linknode *x);
	linknode *Search(int code);
	void Edit();
	void Remove();
	bool operator==(FileLinkList a);
	bool operator>(FileLinkList a);
	bool operator<(FileLinkList a);
	FileLinkList(CharString *word);
	~FileLinkList();
private:
	linknode *search(int code);
};