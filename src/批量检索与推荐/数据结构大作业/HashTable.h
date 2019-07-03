#pragma once
#include "stdio.h"
#include "CharString.h"
#include "CharStringLink.h"

struct character                                      //存储词库用的汉字节点
{
	character **next;//存储指向接下来汉字的指针的数组.后继较少时，直接顺序存储节约空间，较多时，采用哈希节约搜索的时间
	int *hashtable;//当next较小时，存储这些汉字的哈希值
	CharString a;  //该节点对应的汉字
	int length;    //其后继汉字的数量
	bool flag;     //后继是多还是少，多为0，少为1.
	int hashmax;
	bool endtag;//终止标签，即到这个词为止是否构成一个词
};

char* Utf8ToGb2312(char *p);                                   //词库汉字采用uft8编码，将其转化为GB编码

int hash(CharString *str,int position);                         //哈希函数

character *initdictionary(FILE *fp);                           //初始化词典（也是预留接口）

character *addcharacter(character *currentnode,CharString *currentcharacter,int hashcode);     //初始化词典时调用，加入一个汉字（此时上一个字的词语较少）

character *addcharacterlarge(character *currentnode,CharString *currentcharacter,int hashcode,bool &errorflag);//初始化词典时调用，加入一个汉字（此时上一个字的词语较多）

void divideword(CharString *maintext,character *root,FILE *output);           //对整段进行分词的函数（也是预留接口）

void quiry(CharString *maintext,int &original_position,character *root,CharStringLink &result);   //分词时调用，查询一个词是否在词库中

CharStringLink* divideword2(CharString *maintext,character *root);  //推荐时分词