#pragma once
#include <cstdio>
#include <cstdlib>
#include "CharString.h"


struct linknode      //文档链表节点
{
	int filecode;    //文件编号（文件名）
	int num;         //出现次数
	int occurnum;    //出现在这个网页中的不同关键词数量 用于将不同关键词的文档链表合并时按关键词数量排序 建立倒排文档时赋值为1即可
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