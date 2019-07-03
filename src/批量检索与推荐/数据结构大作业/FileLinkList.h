#pragma once
#include <cstdio>
#include <cstdlib>
#include "CharString.h"


struct linknode      //�ĵ�����ڵ�
{
	int filecode;    //�ļ���ţ��ļ�����
	int num;         //���ִ���
	int occurnum;    //�����������ҳ�еĲ�ͬ�ؼ������� ���ڽ���ͬ�ؼ��ʵ��ĵ�����ϲ�ʱ���ؼ����������� ���������ĵ�ʱ��ֵΪ1����
	linknode *next,*pre;//���ǰ��
};

class FileLinkList
{
public:
	CharString key;                     //�ؼ��ʣ���һ������
	linknode *head;                     //ͷ�ڵ�ָ��
	int doc_frequency;                  //�������ĵ���
	void Add(int code);                 //����
	void Add_for_merge(linknode *x);    //�鲢���������������н�����ؼ��ʵ��ĵ�����鲢��
	linknode *Search(int code);         //����
	void Edit(int oldcode,int newcode); //�༭�ڵ�
	void Remove(int code);              //ɾ���ڵ�
	bool operator==(FileLinkList a);    //������˫�Ⱥţ����ں�С�ںţ����ڼ���ƽ����ʱ�Ƚ�
	bool operator>(FileLinkList a);
	bool operator<(FileLinkList a);
	FileLinkList(CharString *word);
	~FileLinkList();
private:
	linknode *search(int code);
};