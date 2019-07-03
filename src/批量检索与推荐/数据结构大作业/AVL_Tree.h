#pragma once
#include <cstdio>
#include <cstdlib>
#include "FileLinkList.h"

struct treenode          //���ڵ�
{
	FileLinkList *data;      //�ؼ���
	int bf;                  //balance factor
	treenode *left,*right;//���Һ���
};

class AVL_Tree
{
public:
	AVL_Tree();
	treenode *root;     
	void Insert(CharString *word,int urlcode);                            //��AVL���в���ؼ���x
	void Adjust(treenode *&currentnode);      
	FileLinkList *Search(CharString &word);                  //�����ؼ���Ϊx�Ľڵ�
	//void Remove(T x);                                     //ɾ��AVL���ֹؼ���Ϊx�Ľ��
	//void Edit(T currentkey,T newkey);                     //��AVL�������еĹؼ���currentkey�޸�Ϊnewkey
private:
	                      //�����ڵ�
	int insert(treenode *&p,CharString *word,int urlcode);
	void adjust(treenode *&p);                //����ʱ�ĵ���
	//int adjust2(treenode *&p);                //ɾ��ʱ�ĵ���
	//int remove(treenode *&p,treenode *&last,T x);
	void R_rotate(treenode *&p);
	void L_rotate(treenode *&p);
};

void init_InvertedFileIndex(AVL_Tree *InvertedFileIndex,FILE *fp,int urlcode,int *word_num);//��ʼ����������
