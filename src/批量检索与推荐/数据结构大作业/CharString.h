#pragma once
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"

class CharString
{
public:
	int* get_next();                                              //KMP算法求失效函数
	int indexOf(CharString &T,int pos,int *next);                  //KMP算法模式匹配，寻找字串位置
	CharString *substring(CharString &a,int pos,int len);           //在字符串a的第pos位开始取len长度的字串，返回这个字串
	void concat(CharString &a,CharString &b);                     //将b连接在a的后面
	void assign(CharString *a);                     //赋值
	void addChar(char x);                                         //向字符串添加一个字符
	void PrintString(FILE *out);                                  //向指定文件中输出该字符串
	char *GetData();                                              //返回字符串对应的chan数组，用于输出操作
	void remove();                                                //删除字符串末尾的一个字符
	void remove2();                                               //删除字符串末尾除'\0'的字符
	char GetAChar(int position);                                  //取第position位置的一个字符
	int GetLength();                                              //获取字符串长度
	bool operator==(CharString &T);                                //重载==符号
	bool operator>(CharString &T);
	bool operator<(CharString &T);  
	void operator=(char *a);                                      //重载=符号，通过一个char数组给字符串直接赋值                        
	int TagDefine();                                              //标签种类判断，用于网页解析，返回0为头标签，1为尾标签，2为无需尾标签结束的标签
	bool TagCheck(CharString head);                               //判断head标签是否是该标签的头标签
	bool SameTag(CharString &b);                                   //判断两种标签是否是同一种标签，如<div>和<div class=""...>就是同一种
	CharString();                                                 //构造
	~CharString();                                                //析构
private:
	char *data;
	int size;
	int length;
	int cursor;
};

char *getfilename(int urlcode);

int geturlcode(CharString *filename);

bool title_or_word(CharString *file);