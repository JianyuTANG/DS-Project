#pragma once
#include <cstdio>
#include <cstdlib>
#include "FileLinkList.h"

struct treenode          //树节点
{
	FileLinkList *data;      //关键字
	int bf;                  //balance factor
	treenode *left,*right;//左右孩子
};

class AVL_Tree
{
public:
	AVL_Tree();
	treenode *root;     
	void Insert(CharString *word,int urlcode);                            //向AVL树中插入关键字x
	void Adjust(treenode *&currentnode);      
	FileLinkList *Search(CharString &word);                  //搜索关键字为x的节点
	//void Remove(T x);                                     //删除AVL树种关键字为x的结点
	//void Edit(T currentkey,T newkey);                     //将AVL树中已有的关键字currentkey修改为newkey
private:
	                      //树根节点
	int insert(treenode *&p,CharString *word,int urlcode);
	void adjust(treenode *&p);                //插入时的调整
	//int adjust2(treenode *&p);                //删除时的调整
	//int remove(treenode *&p,treenode *&last,T x);
	void R_rotate(treenode *&p);
	void L_rotate(treenode *&p);
};

void init_InvertedFileIndex(AVL_Tree *InvertedFileIndex,FILE *fp,int urlcode,int *word_num);//初始化倒排索引
