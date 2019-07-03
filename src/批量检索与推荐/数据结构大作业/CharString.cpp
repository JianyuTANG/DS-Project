#pragma once
#include "CharString.h"

CharString::CharString()
{
	data=(char *)malloc(sizeof(char)*10);
	size=10;
	cursor=0;
	length=0;
}

CharString::~CharString()
{
	if(data!=NULL)
	{
		free(data);
		data=NULL;
	}
}

void CharString::addChar(char x)
{
	if(length==0)
	{
		data[cursor]=x;
		length++;
	}
	else
	{
		cursor++;
		data[cursor]=x;
		length++;
		if(length==size)
		{
			data=(char *)realloc(data,size+30);
			size+=30;
		}
	}
}

int *CharString::get_next()
{
	int *n;
	n=new int[length-1];
	n[0]=0;
	n[1]=0;
	for(int i=2;i<cursor-1;i++)
	{
		int k=n[i-1];
		while(true)
		{
			if(data[i-1]==data[k])
			{
				n[i]=k+1;
				break;
			}
			if(k==0)
			{
				n[i]=0;
				break;
			}
			k=n[k];
		}
	}
	return n;
}

int CharString::indexOf(CharString &T,int pos,int *next)
{
	int k=pos;
	int j=0;
	int Tlenth=T.length;
	while(k<length-1&&j<Tlenth-1)
	{
		if(data[k]==T.data[j])
		{
			k++;
			j++;
		}
		else
		{
			if(j==0)
				k++;
			j=next[j];
		}
	}
	if(j==Tlenth-1)
		return k-j;
	else
		return -1;
}

bool CharString::TagCheck(CharString head)                   //检查两个标签是否构成一对，即它们是对应的头标签和尾标签,如<div>和</div>
{
	int i=2;
	while(data[i]!='>')
	{
		if(head.data[i-1]!=data[i])
			return 0;
		else
			i++;
	}
	return 1;
}

int CharString::TagDefine()                                //判断一个标签是头标签，尾标签，或是既是头也是尾                  
{
	if(data[0]!='<')
		return -1;
	if(data[1]=='/')
		return 1;
	else if(data[1]=='>'||data[1]=='!')
		return 2;
	if(data[cursor-2]=='/')
		return 2;
	return 0;
}

void CharString::PrintString(FILE *out)
{
	CharString special;
	special="&nbsp;";
	int *n_special;
	n_special=special.get_next();
	if(length==0)
		return;
	int i=0;
	int j=cursor-1;
	while(data[i]==' '||data[i]=='\n')
		i++;
	while(data[j]==' '||data[j]=='\n')
		j--;
	if(indexOf(special,0,n_special)==0)
		i+=6;
	int flag=0;
	while(i<=j)
	{
		fprintf(out,"%c",data[i]);
		i++;
		flag=1;
	}
	if(flag==1)
		fprintf(out,"\n");
}

void CharString::concat(CharString &a,CharString &b)
{
	int i=0;
	if(a.length==0)
		;
	else if(a.data[cursor]=='\0')
		a.remove();
	while(i<=b.cursor)
	{
		a.addChar(b.data[i]);
		i++;
	}
	if(a.data[a.cursor]!='\0')
		a.addChar('\0');
}

char *CharString::GetData()
{
	return data;
}

bool CharString::operator==(CharString &T)
{
	int i=0;
	while(i<=cursor)
	{
		if(data[i]!=T.data[i])
			return false;
		i++;
	}
	return true;
}

void CharString::operator=(char *a)
{
	if(data!=NULL)
	{
		cursor=0;
		data=(char *)realloc(data,10*sizeof(char));;
		size=10;
		length=0;
	}
	int i=0;
	while(true)
	{
		addChar(a[i]);
		if(a[i]=='\0')
			break;
		i++;
	}
}

char CharString::GetAChar(int position)
{
	return data[position];
}

void CharString::remove()
{
	if(length==0)
		return;
	if(length==1)
	{
		length--;
		return;
	}
	else
	{
		length--;
		cursor--;
	}
}



CharString *CharString::substring(CharString &a,int pos,int len)
{
	CharString *temp;
	temp=new CharString;
	for(int i=pos;i<pos+len;i++)
	{
		temp->addChar(a.data[i]);
	}
	if(temp->data[temp->cursor]!='\0')
		temp->addChar('\0');
	return temp;
}

bool CharString::SameTag(CharString &b)
{
	int i=0;
	bool answer=1;
	while(data[i]!=' '&&data[i]!='>'&&b.data[i]!='>')
	{
		if(data[i]!=b.data[i])
		{
			answer=0;
			return answer;
		}
		i++;
	}
	return answer;
}

int CharString::GetLength()
{
	return length;
}

void CharString::assign(CharString *a)
{
	char *d=a->data;
	int i=0;
	while(true)
	{
		addChar(d[i]);
		if(d[i]=='\0')
			break;
		i++;
	}
	d=NULL;
}

char *getfilename(int urlcode)
{
	CharString output_filename;                          //输出的文件名
	CharString txt;
	txt=".txt";
	char way[]="input\\";
	int i=0;
	while(way[i]!='\0')
	{
		output_filename.addChar(way[i]);
		i++;
	}
	int j=0;
	char num[10];
	itoa(urlcode,num,10);
	while(num[j]!='\0')
	{
		output_filename.addChar(num[j]);
		j++;
		i++;
	}
	output_filename.concat(output_filename,txt);
	char *tmp=new char[50];
	memcpy(tmp,output_filename.GetData(),output_filename.GetLength()+1);
	return tmp;
}

bool CharString::operator<(CharString &T)
{
	int i=0;
	while(data[i]!='\0'&&T.data[i]!='\0')
	{
		if(data[i]<T.data[i])
			return 1;
		else if(data[i]==T.data[i])
			i++;
		else
			return 0;
	}
	if(data[i]=='\0')
		return 1;
	else
		return 0;
}

bool CharString::operator>(CharString &T)
{
	int i=0;
	while(data[i]!='\0'&&T.data[i]!='\0')
	{
		if(data[i]>T.data[i])
			return 1;
		else if(data[i]==T.data[i])
			i++;
		else
			return 0;
	}
	if(data[i]=='\0')
		return 0;
	else
		return 1;
}

int geturlcode(CharString *file)
{
	char *filename=file->GetData();
	int i=0,j=0;
	while(filename[i]!='\\')
		i++;
	i++;
	char url[10];
	while(filename[i]!='.'&&filename[i]!='t')
	{
		url[j]=filename[i];
		j++;
		i++;
	}
	url[j]='\0';
	int urlcode=atoi(url);   
	return urlcode;
}

bool title_or_word(CharString *file)
{
	char *filename=file->GetData();
	int i=0,j=0;
	while(filename[i]!='\\')
		i++;
	i++;
	while(filename[i]!='.'&&filename[i]!='\0')
	{
		if(filename[i]=='t')
			return true;
		i++;
	}
	return false;
}

void CharString::remove2()
{
	if(length==0)
		return;
	if(data[cursor]=='\0')
		if(cursor-1>=0)
		{
			if(data[cursor-1]=='\n')
			{
				data[cursor-1]='\0';
				cursor--;
				length--;
			}
		}
}