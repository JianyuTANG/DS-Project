#pragma once
#include "HashTable.h"
void InformationExtract(FILE *fp,character *root,FILE *out,FILE *divideword_out,FILE *dividetitle_out);  //遇到包含需要提取的内容，开始提取标题、来源、时间，并引用MainTextExtract提取正文
void TagDistinguish(char *filename,character *root);                               //解析网页的入口。分析标签，遍历所有标签，头标签入栈，尾标签则把最近头标签出栈
void tagdistinguish2(char *filename,character *root);          
void MainTextExtract(FILE *fp,FILE *out,FILE *divideword_out,character *root);     //专门用于提取正文
FILE * PragraphExtract(FILE *fp,FILE *out,FILE *divideword_out,character *root);   //提取正文时调用，用于提取每一个段落，并调用divideword进行分词
void extractInfo(FILE *fp,character *root,FILE *out,FILE *divideword_out); //预留接口
