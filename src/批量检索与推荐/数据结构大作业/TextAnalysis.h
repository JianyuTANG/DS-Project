#pragma once
#include "HashTable.h"
void InformationExtract(FILE *fp,character *root,FILE *out,FILE *divideword_out,FILE *dividetitle_out);  //����������Ҫ��ȡ�����ݣ���ʼ��ȡ���⡢��Դ��ʱ�䣬������MainTextExtract��ȡ����
void TagDistinguish(char *filename,character *root);                               //������ҳ����ڡ�������ǩ���������б�ǩ��ͷ��ǩ��ջ��β��ǩ������ͷ��ǩ��ջ
void tagdistinguish2(char *filename,character *root);          
void MainTextExtract(FILE *fp,FILE *out,FILE *divideword_out,character *root);     //ר��������ȡ����
FILE * PragraphExtract(FILE *fp,FILE *out,FILE *divideword_out,character *root);   //��ȡ����ʱ���ã�������ȡÿһ�����䣬������divideword���зִ�
void extractInfo(FILE *fp,character *root,FILE *out,FILE *divideword_out); //Ԥ���ӿ�
