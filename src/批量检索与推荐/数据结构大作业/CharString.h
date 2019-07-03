#pragma once
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"

class CharString
{
public:
	int* get_next();                                              //KMP�㷨��ʧЧ����
	int indexOf(CharString &T,int pos,int *next);                  //KMP�㷨ģʽƥ�䣬Ѱ���ִ�λ��
	CharString *substring(CharString &a,int pos,int len);           //���ַ���a�ĵ�posλ��ʼȡlen���ȵ��ִ�����������ִ�
	void concat(CharString &a,CharString &b);                     //��b������a�ĺ���
	void assign(CharString *a);                     //��ֵ
	void addChar(char x);                                         //���ַ������һ���ַ�
	void PrintString(FILE *out);                                  //��ָ���ļ���������ַ���
	char *GetData();                                              //�����ַ�����Ӧ��chan���飬�����������
	void remove();                                                //ɾ���ַ���ĩβ��һ���ַ�
	void remove2();                                               //ɾ���ַ���ĩβ��'\0'���ַ�
	char GetAChar(int position);                                  //ȡ��positionλ�õ�һ���ַ�
	int GetLength();                                              //��ȡ�ַ�������
	bool operator==(CharString &T);                                //����==����
	bool operator>(CharString &T);
	bool operator<(CharString &T);  
	void operator=(char *a);                                      //����=���ţ�ͨ��һ��char������ַ���ֱ�Ӹ�ֵ                        
	int TagDefine();                                              //��ǩ�����жϣ�������ҳ����������0Ϊͷ��ǩ��1Ϊβ��ǩ��2Ϊ����β��ǩ�����ı�ǩ
	bool TagCheck(CharString head);                               //�ж�head��ǩ�Ƿ��Ǹñ�ǩ��ͷ��ǩ
	bool SameTag(CharString &b);                                   //�ж����ֱ�ǩ�Ƿ���ͬһ�ֱ�ǩ����<div>��<div class=""...>����ͬһ��
	CharString();                                                 //����
	~CharString();                                                //����
private:
	char *data;
	int size;
	int length;
	int cursor;
};

char *getfilename(int urlcode);

int geturlcode(CharString *filename);

bool title_or_word(CharString *file);