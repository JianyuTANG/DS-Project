#include "CharString.h"
#include "Stack.h"
#include "TextAnalysis.h"


void InformationExtract(FILE *fp,character *root,FILE *out,FILE *divideword_out,FILE *dividetitle_out)
{
	char temp;
	CharString title;                                            //以下均为重要标签的初始化
	title="<h1";
	int *n_title;
	n_title=title.get_next();
	CharString time_source,time_source2,time_source3;
	time_source="post_time_source";
	time_source2="ep-time-soure";
	time_source3="art-about-new";
	int *n_time_source,*n_time_source2,*n_time_source3;
	n_time_source=time_source.get_next();
	n_time_source2=time_source2.get_next();
	n_time_source3=time_source3.get_next();
	CharString ptime;
	ptime="ptime";
	int *n_ptime;
	n_ptime=ptime.get_next();
	CharString maintext,maintext2;
	maintext="post_text";
	maintext2="endText";
	int *n_maintext,*n_maintext2;
	n_maintext=maintext.get_next();
	n_maintext2=maintext2.get_next();
	CharString paragraph,paragraph2;
	paragraph="<p>";
	paragraph2="<p";
	int *n_paragraph2;
	n_paragraph2=paragraph2.get_next();
	CharString endparagraph;
	endparagraph="</p>";
	while(temp=fgetc(fp))                                        //开始读取
	{
		if(feof(fp))
			return;
		if(temp=='<')
		{
			CharString *strtemp;
			strtemp=new CharString;
			strtemp->addChar(temp);
			while(temp=fgetc(fp))
			{
				if(feof(fp))
					return;
				strtemp->addChar(temp);
				if(temp=='>')
					break;
			}
			strtemp->addChar('\0');                             //得到一个标签
			int tag;
			tag=strtemp->TagDefine();                           //判断标签类型
			switch(tag)
			{ 
			case 0:                                             //头标签的情况
				{
					if(strtemp->indexOf(title,0,n_title)==0)    //读取标题
					{
						CharString ttitle;
						while(temp=fgetc(fp))
						{
							if(feof(fp))
								return;
							if(temp=='<')
								break;
							ttitle.addChar(temp);
						}
						ttitle.addChar('\0');
						ttitle.PrintString(dividetitle_out);
						divideword(&ttitle,root,dividetitle_out);
						ttitle.PrintString(out);
					}
					else if(strtemp->indexOf(time_source,1,n_time_source)>1||strtemp->indexOf(time_source2,1,n_time_source2)>1) //读取时间和来源
					{
						CharString time,source;
						temp=fgetc(fp);
						while(temp<48||temp>58)
							temp=fgetc(fp);
						while(temp<=58&&temp>=32)                            //读取时间
						{
							time.addChar(temp);
							temp=fgetc(fp);
							if(feof(fp))
								return;
						}
						time.addChar('\0');
						//time.PrintString(out);
						while(temp!='<')                                     //来源存在超链接，故存在于<a...>与</a>之间
						{
							if(feof(fp))
								return;
							temp=fgetc(fp);
						}
						int flag=0;                                          /*遇到的标签数，理论上来源位于第一个标签之后，第二个标签之前
																			 故当flag==1时，读取信息，flag>1时停止*/
						while(true)                                          //读取来源
						{
							if(temp=='<')                                    //读取标签，并记录这是第几个标签
							{
								flag++;
								CharString sstrtemp;
								sstrtemp.addChar(temp);
								while(temp=fgetc(fp))
								{
									if(feof(fp))
										return;
									sstrtemp.addChar(temp);
									if(temp=='>')
									{
										break;
									}
								}
								sstrtemp.addChar('\0');
								if(sstrtemp.TagCheck(*strtemp)==1)           //遇到对应于<div....>的</div>，整个读取即停止
									break;
							}
							else if(flag==1)                                 //仅当第一个标签之后才是真正的来源内容，需要读取
								source.addChar(temp);
							temp=fgetc(fp);
							if(feof(fp))
								return;
						}
						source.addChar('\0');
						source.PrintString(out);                             //输出来源及时间
						time.PrintString(out);
						//return;
					}
					else if(strtemp->indexOf(time_source3,1,n_time_source3)>1)  //此部分为针对特殊网页6.html标签完全不同而设计
					{
						CharString time,source;
						temp=fgetc(fp);
						int stopflag=0;
						while(true)
						{
							if(temp=='<')
							{
								CharString sstrtemp;
								sstrtemp.addChar(temp);
								while(temp=fgetc(fp))
								{
									sstrtemp.addChar(temp);
									if(temp=='>')
										break;
								}
								sstrtemp.addChar('\0');
								if(stopflag==0&&sstrtemp.indexOf(paragraph2,0,n_paragraph2)==0)
								{
									temp=fgetc(fp);
									while(temp!='<')
									{
										if(temp!=' '&&temp!='\n')
											source.addChar(temp);
										temp=fgetc(fp);
									}
									source.addChar('\0');
									source.PrintString(out);
									stopflag=1;
								}
								else if(sstrtemp.indexOf(ptime,4,n_ptime)>4)
								{
									temp=fgetc(fp);
									while(temp<0||temp>128)
										temp=fgetc(fp);
									while(temp>0&&temp<128)
									{
										if(temp=='<')
											break;
										time.addChar(temp);
										temp=fgetc(fp);
									}
									time.addChar('\0');
									time.PrintString(out);
									stopflag++;
								}
							}
							if(stopflag==2)
								break;
							temp=fgetc(fp);
							if(feof(fp))
								return;
						}
					}
					else if(strtemp->indexOf(maintext,1,n_maintext)>1||strtemp->indexOf(maintext2,1,n_maintext2)>1)       //读取正文部分
					{
						MainTextExtract(fp,out,divideword_out,root);     //调用MainTextExtract读取正文部分
						return;
					}
					break;
				}
			case 1:
				break;
			case 2:
				delete strtemp;
				break;
			}

		}
	}
}


/*
void TagDistinguish(char *filename,character *root)                       //分析标签，遍历所有标签，头标签入栈，尾标签则把最近头标签出栈
{
CharString html;
html="</html>";
CharString mainbody,mainbody2,maintext2,mainbody3;                       //重要标签的初始化，用来进行判断
mainbody="post_content_main";
mainbody2="ep-content-main";
mainbody3="g-wrap";
maintext2="endText";
int *n_mainbody,*n_mainbody2,*n_maintext2,*n_mainbody3;
n_mainbody=mainbody.get_next();
n_mainbody2=mainbody2.get_next();
n_maintext2=maintext2.get_next();
n_mainbody3=mainbody3.get_next();
FILE *fp;               
if(fp=fopen(filename,"r"))                                               //打开网页文件
;
CharString output_filename,divideword_filename;                          //输出的文件名
CharString info,txt;
info=".info";
txt=".txt";
char way[]="output\\";
int i=0;
while(way[i]!='\0')
{
output_filename.addChar(way[i]);
divideword_filename.addChar(way[i]);
i++;
}
i=0;
while(filename[i]!='\\')
i++;
i++;
while(filename[i]!='.')
{
output_filename.addChar(filename[i]);
divideword_filename.addChar(filename[i]);
i++;
}
output_filename.concat(output_filename,info);
divideword_filename.concat(divideword_filename,txt);
FILE *out,*divideword_out;                                              //创建分词结果与网页解析结果的文件
if((out=fopen(output_filename.GetData(),"w"))==0)
{
printf("fail\n");
printf("%s\n",output_filename.GetData());
}
else
printf("%s\n",output_filename.GetData());
if((divideword_out=fopen(divideword_filename.GetData(),"w"))==0)
{
printf("fail\n");
}
else
printf("%s\n",divideword_filename.GetData());                       
char temp;
Stack<CharString> s;
int special_web=0;
while(temp=fgetc(fp))                                                  //开始读取网页
{
int flag=0;    //停止循环标志
if(temp=='<')
{
int tag=0;            

/*表示标签的类型:
tag=0说明该标签为前置标签，压入栈；
tag=1说明该标签为后置标签,检查栈顶标签，若是对应的前置标签,则出栈;
tag=2既是前标签又是后标签,如<meta http-equiv="expires" content="0"/>,即自动消亡，无需入栈。*/
/*
CharString *strtemp;
strtemp=new CharString;
strtemp->addChar(temp);
while(temp=fgetc(fp))
{
strtemp->addChar(temp);
if(temp=='>')
{
break;
}
}
//至此得到一个完整的标签，并存入字符串类(CharString) 指针strtemp中

tag=strtemp->TagDefine();               //计算这个标签的类型
switch(tag)
{
case 0:                                  //这个标签是头标签
if(strtemp->indexOf(mainbody,1,n_mainbody)>1||strtemp->indexOf(mainbody2,1,n_mainbody2)>1||strtemp->indexOf(mainbody3,1,n_mainbody3)>1)   //如果这个标签包含post_content_main，即说明是需要提取的信息
{
if(strtemp->indexOf(mainbody3,1,n_mainbody3)>1)
special_web++;
InformationExtract(fp,root,out,divideword_out);

}
else if(strtemp->indexOf(maintext2,1,n_maintext2)>1)      //遇到有两段正文（即存在EndText）的情况，单独读取一遍正文
{
if(special_web>0)
break;
MainTextExtract(fp,out,divideword_out,root);
}
s.push(*strtemp);
break;
case 1:                                  //这个标签是尾标签
while(true)
{
if(s.empty()!=0)
{ 
/*通过TagCheck函数，比对该标签是否是栈顶元素的尾标签，是则出栈;
否则说明栈内有标签未正常关闭，则令其出栈（人工关闭），直到栈顶为对应的标签为止*//*
if(strtemp->TagCheck(s.elemOnTheTop())==1) 
{
s.pop();                //出栈
break;
}
else
s.pop();
}
else
break;
}
if(*strtemp==html)                   //如果这个标签是</html>说明达到整个网页末尾，标志flag=1，提示循环结束读取信息
{
flag=1;
}
delete strtemp;
break;
case 2:                                  //这个标签自己结束,例如<..../>，无需入栈或出栈操作
delete strtemp;
break;
}
}
if(flag==1)                                  //结束读取信息
break;
}
}
*/

void tagdistinguish2(char *filename,character *root)        //改进后的解析入口
{
	CharString html;
	html="</html>";
	CharString mainbody,mainbody2,maintext2,mainbody3;                       //重要标签的初始化，用来进行判断
	mainbody="post_content_main";
	mainbody2="ep-content-main";
	mainbody3="g-wrap";
	maintext2="endText";
	int *n_mainbody,*n_mainbody2,*n_maintext2,*n_mainbody3;
	n_mainbody=mainbody.get_next();
	n_mainbody2=mainbody2.get_next();
	n_maintext2=maintext2.get_next();
	n_mainbody3=mainbody3.get_next();
	FILE *fp;               
	if(fp=fopen(filename,"r"))                                               //打开网页文件
		;
	else
	{
		printf("fail\n");
		return;
	}
	CharString output_filename,divideword_filename,title_divide_filename;                          //输出的文件名
	CharString info,txt;
	info=".info";
	txt=".txt";
	char way[]="output\\";
	int i=0;
	while(way[i]!='\0')
	{
		output_filename.addChar(way[i]);
		divideword_filename.addChar(way[i]);
		title_divide_filename.addChar(way[i]);
		i++;
	}
	i=0;
	while(filename[i]!='\\')
		i++;
	i++;
	int j=0;
	char url[50];
	while(filename[i]!='.')
	{
		url[j]=filename[i];
		j++;
		output_filename.addChar(filename[i]);
		divideword_filename.addChar(filename[i]);
		title_divide_filename.addChar(filename[i]);
		i++;
	}
	title_divide_filename.addChar('t');
	url[j]='\0';
	//urlcode=atoi(url);
	//word_num[urlcode]=0;
	output_filename.concat(output_filename,info);
	divideword_filename.concat(divideword_filename,txt);
	title_divide_filename.concat(title_divide_filename,txt);
	FILE *out,*divideword_out,*dividetitleout;                                         //创建分词结果与网页解析结果的文件
	if((out=fopen(output_filename.GetData(),"w"))==0)
	{
		printf("fail\n");
		return;
		printf("%s\n",output_filename.GetData());
	}
	else
		printf("%s\n",output_filename.GetData());
	if((divideword_out=fopen(divideword_filename.GetData(),"w"))==0)
	{
		printf("fail\n");
		return;
	}
	else
		printf("%s\n",divideword_filename.GetData());
	if((dividetitleout=fopen(title_divide_filename.GetData(),"w"))==0)
	{
		printf("fail\n");
		return;
	}
	else
		printf("%s\n",title_divide_filename.GetData());
	char temp;
	for(;!feof(fp);)
	{
		temp=fgetc(fp);
		if(temp=='<')
		{
			CharString strtemp;
			strtemp.addChar(temp);
			temp=fgetc(fp);
			if(temp=='/')
				continue;
			strtemp.addChar(temp);
			while(temp=fgetc(fp))
			{
				if(feof(fp))
				{
					fclose(fp);
					fclose(out);
					fclose(divideword_out);
					return;
				}
				strtemp.addChar(temp);
				if(temp=='>')
					break;
			}
			if(strtemp.indexOf(mainbody,1,n_mainbody)>1||strtemp.indexOf(mainbody2,1,n_mainbody2)>1||strtemp.indexOf(mainbody3,1,n_mainbody3)>1)   //如果这个标签包含post_content_main，即说明是需要提取的信息
				InformationExtract(fp,root,out,divideword_out,dividetitleout);
			else if(strtemp.indexOf(maintext2,1,n_maintext2)>1)      //遇到有两段正文（即存在EndText）的情况，单独读取一遍正文
				MainTextExtract(fp,out,divideword_out,root);
		}
	}
	fclose(fp);
	fclose(out);
	fclose(divideword_out);
	fclose(dividetitleout);
}

void MainTextExtract(FILE *fp,FILE *out,FILE *divideword_out,character *root)
{
	CharString paragraph;                           //重要标签的初始化
	paragraph="<p>";
	CharString endparagraph;
	endparagraph="</p>";
	CharString div;
	div="div";
	CharString enddiv;
	enddiv="</div>";
	char temp;
	//CharString maintext;
	Stack<CharString> s;                            //用于处理标签嵌套的栈s
	while(temp=fgetc(fp))
	{
		if(feof(fp))
			return;
		int flag=0;
		if(temp=='<')
		{
			CharString strtemp;
			int tag;
			strtemp.addChar(temp);
			while(temp=fgetc(fp))
			{
				if(feof(fp))
					return;
				strtemp.addChar(temp);
				if(temp=='>')
					break;
			}
			strtemp.addChar('\0');                                           //读取一个标签
			tag=strtemp.TagDefine();
			switch(tag)
			{
			case 0:  //头标签
				if(strtemp==paragraph)                                       //头标签是<p>，即一段文字的开始，开始读取内容
					PragraphExtract(fp,out,divideword_out,root);
				else if(enddiv.TagCheck(strtemp)==1)                         //头标签是<div...>，说明有嵌套，需要栈来判断在哪一个</div>处结束读取
					s.push(strtemp);
				break;
			case 1:  //尾标签
				if(strtemp==enddiv)
				{
					if(s.empty()!=0)
						s.pop();                                             //是</div>需要将栈中的<div>退栈
					else
						flag=1;                                              //若栈已空，说明这个</div>表示整个正文部分结束
				}
				break;
			case 2:
				break;
			}
		}
		if(flag==1)
			break;
	}
	//maintext.PrintString(out);
}

FILE * PragraphExtract(FILE *fp,FILE *out,FILE *divideword_out,character *root)
{
	CharString endparagraph;                                 //重要标签初始化
	endparagraph="</p>";
	CharString endsuperlink;
	endsuperlink="</a>";
	CharString font;
	font="<font>";
	CharString enddiv;
	enddiv="</div>";
	CharString startdiv;
	startdiv="<div";
	CharString endmaintext;
	endmaintext="ep-source";
	int *n_endmaintext;
	n_endmaintext=endmaintext.get_next();
	CharString paragraph;                                     //该段内容，最后需要将其打印到文件中
	Stack<CharString> s;
	bool open_or_close=0;
	char x;
	int cengshu=0;
	while(true)                                              //开始读取，每一部分分为标签或标签外部分两种
	{
		bool tag_or_not=0;                                   //0为标签间内容，1为标签
		CharString *temp;
		temp=new CharString;
		x=fgetc(fp);
		if(open_or_close==1)
		{
			tag_or_not=1;
			temp->addChar('<');
		}
		open_or_close=0;
		if(x!=' '&&x!='\n'&&x!='	'&&x!='　')               
			temp->addChar(x);
		while(true)
		{
			if(feof(fp))
				return NULL;
			x=fgetc(fp);
			if(x=='<')
			{
				open_or_close=1;
				break;
			}
			else if(x=='>')
			{
				temp->addChar(x);
				tag_or_not=1;                   //这是一个标签
				break;
			}
			if(tag_or_not==0)
			{
				if(x!=' '&&x!='\n'&&x!='	'&&x!='　')//过滤空格和回车
					temp->addChar(x);
			}
			else
				temp->addChar(x);
		}
		temp->addChar('\0');
		if(tag_or_not==0)                       //是标签间内容
		{
			if(s.empty()==0)
				paragraph.concat(paragraph,*temp);
			else if(endsuperlink.TagCheck(s.elemOnTheTop())==1||s.elemOnTheTop()==font)
				paragraph.concat(paragraph,*temp);
		}
		else                                    //是一个标签，处理原则：除了表示超链接的标签，其他标签嵌套中的内容一律不输出
		{
			int tagtype;
			tagtype=temp->TagDefine();          //判断标签类型
			switch(tagtype)
			{
			case 0:                             //头标签
				if(temp->indexOf(endmaintext,1,n_endmaintext)>3) //是ep-source标签，用于提示结束正文读取（只使用div判断可能有标签不闭合情况，此为保险）
				{
					if(paragraph.GetLength()>3)                 //如果段落过段，说明有可能是单个符号，则过滤掉，下同
					{
						paragraph.PrintString(out);
						divideword(&paragraph,root,divideword_out);
					}
					return fp;
				}
				if(endparagraph.TagCheck(*temp)==1)             //是一个头标签，判断嵌套层数
				{
					cengshu++;
					break;
				}
				if(s.empty()==0)                                //栈为空则压栈
				{
					s.push(*temp);
				}
				else
				{
					if(temp->SameTag(s.elemOnTheTop()))         //如果与栈中标签是同一种，则压栈（栈中只能有一种标签）
						s.push(*temp);
					else
						delete temp;
				}
				break;
			case 1:                                             //是尾标签
				if(s.empty()==0)                                //栈为空的情况
				{
					if(*temp==enddiv)                           //遇到</div>自动停止读取
					{
						delete temp;
						return fp;
					}
					else if(*temp==endparagraph)                //遇到</p>判断嵌套层数，如果是最外层的，则说明该段落结束
					{
						delete temp;
						if(paragraph.GetLength()>3)
						{
							paragraph.PrintString(out);
							divideword(&paragraph,root,divideword_out);
						}
						return fp;
					}
				}
				else                                            //栈不为空
				{
					if(temp->TagCheck(s.elemOnTheTop())==1)     //是栈中标签对应的尾标签，则出栈
						s.pop();
					else if(*temp==endparagraph)                //是</p>则嵌套层数减一
					{
						cengshu--;
						if(cengshu<0)                           //已无<p>的嵌套，则说明该段落结束，直接输出
						{
							if(paragraph.GetLength()>3)
							{
								paragraph.PrintString(out);
								divideword(&paragraph,root,divideword_out);
							}
							delete temp;
							return fp;
						}
					}
					delete temp;
				}
				break;
			case 2:
				delete temp;
				break;
			}
		}
	}
	return fp;
}