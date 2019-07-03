#include "HashTable.h"
#include "windows.h"

char* Utf8ToGb2312(char *p)
{
	DWORD dwNum = MultiByteToWideChar(CP_UTF8,0,p,-1,NULL,0);
	char *psText;                                                         //存储文件名
	wchar_t *pwText=(wchar_t*)malloc(dwNum*sizeof(wchar_t));
	dwNum = MultiByteToWideChar(CP_UTF8,0,p,-1,pwText,dwNum);
	dwNum = WideCharToMultiByte(CP_ACP,0,pwText,-1,NULL,0,NULL,NULL);
	psText=(char*)malloc(dwNum*sizeof(char));
	dwNum = WideCharToMultiByte(CP_ACP,0,pwText,-1,psText,dwNum,NULL,NULL);
	free(pwText);
	return psText;
}

int hash(CharString str,int position)                                       //哈希函数
{
	int hash=0;
	char a,b;
	a=str.GetAChar(position);
	position++;
	b=str.GetAChar(position);
	hash=((unsigned char)a-129)*190+((unsigned char)b-64)-(unsigned char)b/128;
	return hash;  
}

character *initdictionary(FILE *fp)                                        //词典的初始化
{
	character *root;                                                       //创建根节点
	character *currentnode;                                                //指向当前节点的指针
	root=new character;
	root->flag=0;
	root->next=(character **)malloc(sizeof(character *)*23500);            //开辟存储空间，用于哈希表
	root->length=23500;
	root->endtag=false;
	for(int i=0;i<23500;i++)
		root->next[i]=NULL;
	char *UniCodetemp;
	UniCodetemp=new char[41];
	UniCodetemp=fgets(UniCodetemp,40,fp);

	while(UniCodetemp=fgets(UniCodetemp,40,fp))                            //按行从词库中读取
	{
		char *tang;
		tang=UniCodetemp;
		currentnode=root;
		char *GBtemp;
		GBtemp=Utf8ToGb2312(UniCodetemp);                                  //转码成国标编码
		CharString word;
		int position=0;
		word=GBtemp;                                                       //得到改行对应的词语
		int lengthofword=0;
		while(word.GetAChar(position)!='\n'&&word.GetAChar(position)!='\0')//将这个词语一个一个汉字加进词典
		{
			lengthofword++;
			int hashcode;
			hashcode=hash(word,position);             
			CharString currentcharacter;                                   //当前汉字
			for(int i=0;i<2;i++)                                           //读取当前汉字
			{
				currentcharacter.addChar(word.GetAChar(position));
				position++;
			}
			currentcharacter.addChar('\0');

			if(currentnode->flag==1)                                        //当前节点的后继较少，不使用哈希表存储指向后继的指针
			{
				currentnode=addcharacter(currentnode,currentcharacter,hashcode);
			}
			else                                                           //当前节点的后继较多，已经采用哈希表存储指向后继的指针
			{
				bool errorflag=0;
				currentnode=addcharacterlarge(currentnode,currentcharacter,hashcode,errorflag);
				if(errorflag==1)
					break;
			}
		}
		currentnode->endtag=true;                                          //一个词语在这个字结束的标志
		if(lengthofword==1)
		{
			currentnode->endtag=false;                                     //词典中存在换行符和空格的特殊情况处理，不能有一个字的词出现
		}
	}
	printf("The dictionary has been initialized successfully.\n");
	root->endtag=false;
	return root;
}

character *addcharacter(character *currentnode,CharString currentcharacter,int hashcode)
{
	character *temp;
	temp=new character;
	temp->a=currentcharacter.GetData();
	temp->flag=1;
	temp->length=0;
	temp->next=NULL;
	temp->hashmax=0;
	temp->endtag=false;
	if(currentnode->length==0)                                    //当前节点尚无后继
	{
		currentnode->next=new character*[20];
		currentnode->next[0]=temp;
		currentnode->length=1;
		currentnode->hashtable=new int[20];
		currentnode->hashtable[0]=hashcode;
		currentnode->hashmax=hashcode;
		currentnode=currentnode->next[0];
	}
	else if(currentnode->length<=19)                             //当前节点后继较少，不适用哈希表
	{
		int existflag=0;
		for(int i=0;i<currentnode->length;i++)
		{
			if(currentnode->next[i]->a==currentcharacter)        //该汉字对应后继节点已存在
			{
				delete temp;
				currentnode=currentnode->next[i];
				existflag=1;
				break;
			}
		}
		if(existflag==0)                                        //该汉字对应后继节点不存在
		{
			currentnode->next[currentnode->length]=temp;        //将指向这个汉字的指针加入指针域中
			currentnode->hashtable[currentnode->length]=hashcode;
			if(hashcode>currentnode->hashmax)
				currentnode->hashmax=hashcode;
			currentnode->length=currentnode->length+1;
			currentnode=currentnode->next[(currentnode->length)-1];
		}
	}
	else                                                         //节点变多，转而采用哈希表
	{
		int existflag=0;
		for(int i=0;i<currentnode->length;i++)
		{
			if(currentnode->next[i]->a==currentcharacter)        //后继节点已存在
			{
				delete temp;
				currentnode=currentnode->next[i];
				existflag=1;
				break;
			}
		}
		if(existflag==0)                                         //后继节点不存在
		{
			currentnode->flag=0;
			if(hashcode>currentnode->hashmax)
				currentnode->hashmax=hashcode;
			character **tempnext;
			tempnext=(character **)malloc(sizeof(character *)*(currentnode->hashmax+100));
			for(int i=0;i<currentnode->hashmax+100;i++)
			{
				tempnext[i]=NULL;
			}

			for(int i=0;i<=19;i++)
			{
				if(tempnext[currentnode->hashtable[i]]!=NULL)
					printf("ERROR ERROE ERROE\n");
				tempnext[currentnode->hashtable[i]]=currentnode->next[i];
			}
			tempnext[hashcode]=temp;
			currentnode->length=currentnode->hashmax+100;
			delete currentnode->hashtable;
			delete currentnode->next;
			currentnode->next=NULL;
			currentnode->next=tempnext;
			currentnode=currentnode->next[hashcode];
			currentnode->length=0;
		}
	}
	return currentnode;
}

character *addcharacterlarge(character *currentnode,CharString currentcharacter,int hashcode,bool &errorflag)
{
	character *temp;
	temp=new character;
	temp->a=currentcharacter.GetData();
	temp->flag=1;
	temp->length=0;
	temp->next=NULL;
	temp->hashmax=0;
	temp->endtag=false;
	if(hashcode>=currentnode->length)                              //哈希表过小 需要扩展
	{
		currentnode->hashmax=hashcode;
		currentnode->next=(character **)realloc(currentnode->next,sizeof(character *)*(hashcode+200));
		for(int i=currentnode->length;i<hashcode+200;i++)
			currentnode->next[i]=NULL;
		currentnode->length=hashcode+200;
		currentnode->next[hashcode]=temp;
		currentnode=currentnode->next[hashcode];
		currentnode->length=0;
	}
	else                                                           //哈希表足够大，直接加入
	{
		if(currentnode->next[hashcode]!=NULL)                      //该汉字对应的节点已存在
		{
			if(currentnode->next[hashcode]->a==currentcharacter)   //是同一个汉字
			{
				currentnode=currentnode->next[hashcode];
			}
			else                                                   //不是同一个汉字（即哈希冲突）这种情况理论上不会发生
			{                                                      //哈希函数的设计保证了GB2312的汉字不会冲突，此为保险
				errorflag=1;
				printf("ERROR ERROR ERROR ERROR ERROR\n");
			}
		}
		else                                                       //该汉字对应的节点不存在，需要新建节点并加入指针域中
		{
			currentnode->next[hashcode]=temp;
			if(hashcode>currentnode->hashmax)
				currentnode->hashmax=hashcode;
			currentnode=currentnode->next[hashcode];
			currentnode->length=0;
		}
	}
	return currentnode;
}

void divideword(CharString *maintext,character *root,FILE *output)
{
	CharStringLink result;                             //存储分词结果的字符串链表
	int position=0;
	while(maintext->GetAChar(position)!='\0')
	{
		if(maintext->GetAChar(position)>=0&&maintext->GetAChar(position)<=127)  //自动忽略英文、数字、ASCII字符
		{
			position++;
		}
		else                                            //汉字或字符
		{
			quiry(maintext,position,root,result);
		}
	}
	result.print_link(output);                          //向文件中打印分词结果
}

void quiry(CharString *maintext,int &original_position,character *root,CharStringLink &result)
{
	CharString currentword;                        //当前词语
	character *currentnode;                        //进入词典树中的当前节点
	currentnode=root;
	int endposition=0;
	int looptimes=0;
	int position=original_position;
	while(maintext->GetAChar(position)<0||maintext->GetAChar(position)>128)
	{
		looptimes++;
		CharString currentcharacter;               //存储当前汉字
		for(int i=0;i<2;i++)                       //读取当前汉字
		{
			currentcharacter.addChar(maintext->GetAChar(position));
			position++;
		}
		currentcharacter.addChar('\0');            //得到当前汉字
		int hashcode;
		hashcode=hash(currentcharacter,0);         //对该汉字求哈希值
		bool find=0;
		switch(currentnode->flag)                  //判断当前节点的指针域是使用顺序表还是哈希表存储指针
		{
		case 1:                                    //顺序表
			for(int i=0;i<currentnode->length;i++) //遍历即可
			{
				if(currentnode->next[i]->a==currentcharacter)    //发现该汉字
				{
					find=1;
					currentword.concat(currentword,currentcharacter);
					currentnode=currentnode->next[i];
					if(currentnode->endtag==1)
						endposition=looptimes;
					break;
				}
			}
			break;
		case 0:                                    //哈希表
			if(hashcode>currentnode->length||hashcode<0)   //确认该汉字的哈希值不超过哈希表的大小，防止数组越界访问
				break;
			else
			{
				if(currentnode->next[hashcode]!=NULL)      //发现该汉字
				{
					find=1;
					currentword.concat(currentword,currentcharacter);
					currentnode=currentnode->next[hashcode];
					if(currentnode->endtag==1)
						endposition=looptimes;
				}
			}
			break;
		}
		if(find==0)
			break;
	}
	if(currentnode->endtag==0)                      //结束查询字典
	{
		if(endposition!=0)
		{
			currentword=currentword.substring(currentword,0,2*endposition);
			original_position+=2*endposition;
			result.add(currentword);               //向分词结果的字符串链表中加入这个词
		}
		else
			original_position+=2;;
	}
	else if(currentnode->endtag==1)
	{
		original_position+=2*endposition;
		result.add(currentword);
	}
}