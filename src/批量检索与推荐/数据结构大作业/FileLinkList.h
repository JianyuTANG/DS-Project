#pragma once
#include <cstdio>
#include <cstdlib>
#include "CharString.h"


struct linknode      //文档链表节点
{
	int filecode;    //文件编号（文件名）
	int num;         //出现次数
	int occurnum;    //出现在这个网页中的不同关键词数量 用于将不同关键词的文档链表合并时按关键词数量排序 建立倒排文档时赋值为1即可
	linknode *next,*pre;//后继前驱
};

class FileLinkList
{
public:
	CharString key;                     //关键词，即一个词语
	linknode *head;                     //头节点指针
	int doc_frequency;                  //包含的文档数
	void Add(int code);                 //加入
	void Add_for_merge(linknode *x);    //归并（用于批量搜索中将多个关键词的文档链表归并）
	linknode *Search(int code);         //查找
	void Edit(int oldcode,int newcode); //编辑节点
	void Remove(int code);              //删除节点
	bool operator==(FileLinkList a);    //重载了双等号，大于和小于号，便于加入平衡树时比较
	bool operator>(FileLinkList a);
	bool operator<(FileLinkList a);
	FileLinkList(CharString *word);
	~FileLinkList();
private:
	linknode *search(int code);
};