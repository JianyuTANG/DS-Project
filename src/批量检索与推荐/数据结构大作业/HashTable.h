#pragma once
#include "stdio.h"
#include "CharString.h"
#include "CharStringLink.h"

struct character                                      //�洢�ʿ��õĺ��ֽڵ�
{
	character **next;//�洢ָ����������ֵ�ָ�������.��̽���ʱ��ֱ��˳��洢��Լ�ռ䣬�϶�ʱ�����ù�ϣ��Լ������ʱ��
	int *hashtable;//��next��Сʱ���洢��Щ���ֵĹ�ϣֵ
	CharString a;  //�ýڵ��Ӧ�ĺ���
	int length;    //���̺��ֵ�����
	bool flag;     //����Ƕ໹���٣���Ϊ0����Ϊ1.
	int hashmax;
	bool endtag;//��ֹ��ǩ�����������Ϊֹ�Ƿ񹹳�һ����
};

char* Utf8ToGb2312(char *p);                                   //�ʿ⺺�ֲ���uft8���룬����ת��ΪGB����

int hash(CharString *str,int position);                         //��ϣ����

character *initdictionary(FILE *fp);                           //��ʼ���ʵ䣨Ҳ��Ԥ���ӿڣ�

character *addcharacter(character *currentnode,CharString *currentcharacter,int hashcode);     //��ʼ���ʵ�ʱ���ã�����һ�����֣���ʱ��һ���ֵĴ�����٣�

character *addcharacterlarge(character *currentnode,CharString *currentcharacter,int hashcode,bool &errorflag);//��ʼ���ʵ�ʱ���ã�����һ�����֣���ʱ��һ���ֵĴ���϶ࣩ

void divideword(CharString *maintext,character *root,FILE *output);           //�����ν��зִʵĺ�����Ҳ��Ԥ���ӿڣ�

void quiry(CharString *maintext,int &original_position,character *root,CharStringLink &result);   //�ִ�ʱ���ã���ѯһ�����Ƿ��ڴʿ���

CharStringLink* divideword2(CharString *maintext,character *root);  //�Ƽ�ʱ�ִ�