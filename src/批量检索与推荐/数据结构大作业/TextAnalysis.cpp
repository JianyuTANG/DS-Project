#include "CharString.h"
#include "Stack.h"
#include "TextAnalysis.h"


void InformationExtract(FILE *fp,character *root,FILE *out,FILE *divideword_out,FILE *dividetitle_out)
{
	char temp;
	CharString title;                                            //���¾�Ϊ��Ҫ��ǩ�ĳ�ʼ��
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
	while(temp=fgetc(fp))                                        //��ʼ��ȡ
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
			strtemp->addChar('\0');                             //�õ�һ����ǩ
			int tag;
			tag=strtemp->TagDefine();                           //�жϱ�ǩ����
			switch(tag)
			{ 
			case 0:                                             //ͷ��ǩ�����
				{
					if(strtemp->indexOf(title,0,n_title)==0)    //��ȡ����
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
					else if(strtemp->indexOf(time_source,1,n_time_source)>1||strtemp->indexOf(time_source2,1,n_time_source2)>1) //��ȡʱ�����Դ
					{
						CharString time,source;
						temp=fgetc(fp);
						while(temp<48||temp>58)
							temp=fgetc(fp);
						while(temp<=58&&temp>=32)                            //��ȡʱ��
						{
							time.addChar(temp);
							temp=fgetc(fp);
							if(feof(fp))
								return;
						}
						time.addChar('\0');
						//time.PrintString(out);
						while(temp!='<')                                     //��Դ���ڳ����ӣ��ʴ�����<a...>��</a>֮��
						{
							if(feof(fp))
								return;
							temp=fgetc(fp);
						}
						int flag=0;                                          /*�����ı�ǩ������������Դλ�ڵ�һ����ǩ֮�󣬵ڶ�����ǩ֮ǰ
																			 �ʵ�flag==1ʱ����ȡ��Ϣ��flag>1ʱֹͣ*/
						while(true)                                          //��ȡ��Դ
						{
							if(temp=='<')                                    //��ȡ��ǩ������¼���ǵڼ�����ǩ
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
								if(sstrtemp.TagCheck(*strtemp)==1)           //������Ӧ��<div....>��</div>��������ȡ��ֹͣ
									break;
							}
							else if(flag==1)                                 //������һ����ǩ֮�������������Դ���ݣ���Ҫ��ȡ
								source.addChar(temp);
							temp=fgetc(fp);
							if(feof(fp))
								return;
						}
						source.addChar('\0');
						source.PrintString(out);                             //�����Դ��ʱ��
						time.PrintString(out);
						//return;
					}
					else if(strtemp->indexOf(time_source3,1,n_time_source3)>1)  //�˲���Ϊ���������ҳ6.html��ǩ��ȫ��ͬ�����
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
					else if(strtemp->indexOf(maintext,1,n_maintext)>1||strtemp->indexOf(maintext2,1,n_maintext2)>1)       //��ȡ���Ĳ���
					{
						MainTextExtract(fp,out,divideword_out,root);     //����MainTextExtract��ȡ���Ĳ���
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
void TagDistinguish(char *filename,character *root)                       //������ǩ���������б�ǩ��ͷ��ǩ��ջ��β��ǩ������ͷ��ǩ��ջ
{
CharString html;
html="</html>";
CharString mainbody,mainbody2,maintext2,mainbody3;                       //��Ҫ��ǩ�ĳ�ʼ�������������ж�
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
if(fp=fopen(filename,"r"))                                               //����ҳ�ļ�
;
CharString output_filename,divideword_filename;                          //������ļ���
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
FILE *out,*divideword_out;                                              //�����ִʽ������ҳ����������ļ�
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
while(temp=fgetc(fp))                                                  //��ʼ��ȡ��ҳ
{
int flag=0;    //ֹͣѭ����־
if(temp=='<')
{
int tag=0;            

/*��ʾ��ǩ������:
tag=0˵���ñ�ǩΪǰ�ñ�ǩ��ѹ��ջ��
tag=1˵���ñ�ǩΪ���ñ�ǩ,���ջ����ǩ�����Ƕ�Ӧ��ǰ�ñ�ǩ,���ջ;
tag=2����ǰ��ǩ���Ǻ��ǩ,��<meta http-equiv="expires" content="0"/>,���Զ�������������ջ��*/
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
//���˵õ�һ�������ı�ǩ���������ַ�����(CharString) ָ��strtemp��

tag=strtemp->TagDefine();               //���������ǩ������
switch(tag)
{
case 0:                                  //�����ǩ��ͷ��ǩ
if(strtemp->indexOf(mainbody,1,n_mainbody)>1||strtemp->indexOf(mainbody2,1,n_mainbody2)>1||strtemp->indexOf(mainbody3,1,n_mainbody3)>1)   //��������ǩ����post_content_main����˵������Ҫ��ȡ����Ϣ
{
if(strtemp->indexOf(mainbody3,1,n_mainbody3)>1)
special_web++;
InformationExtract(fp,root,out,divideword_out);

}
else if(strtemp->indexOf(maintext2,1,n_maintext2)>1)      //�������������ģ�������EndText���������������ȡһ������
{
if(special_web>0)
break;
MainTextExtract(fp,out,divideword_out,root);
}
s.push(*strtemp);
break;
case 1:                                  //�����ǩ��β��ǩ
while(true)
{
if(s.empty()!=0)
{ 
/*ͨ��TagCheck�������ȶԸñ�ǩ�Ƿ���ջ��Ԫ�ص�β��ǩ�������ջ;
����˵��ջ���б�ǩδ�����رգ��������ջ���˹��رգ���ֱ��ջ��Ϊ��Ӧ�ı�ǩΪֹ*//*
if(strtemp->TagCheck(s.elemOnTheTop())==1) 
{
s.pop();                //��ջ
break;
}
else
s.pop();
}
else
break;
}
if(*strtemp==html)                   //��������ǩ��</html>˵���ﵽ������ҳĩβ����־flag=1����ʾѭ��������ȡ��Ϣ
{
flag=1;
}
delete strtemp;
break;
case 2:                                  //�����ǩ�Լ�����,����<..../>��������ջ���ջ����
delete strtemp;
break;
}
}
if(flag==1)                                  //������ȡ��Ϣ
break;
}
}
*/

void tagdistinguish2(char *filename,character *root)        //�Ľ���Ľ������
{
	CharString html;
	html="</html>";
	CharString mainbody,mainbody2,maintext2,mainbody3;                       //��Ҫ��ǩ�ĳ�ʼ�������������ж�
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
	if(fp=fopen(filename,"r"))                                               //����ҳ�ļ�
		;
	else
	{
		printf("fail\n");
		return;
	}
	CharString output_filename,divideword_filename,title_divide_filename;                          //������ļ���
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
	FILE *out,*divideword_out,*dividetitleout;                                         //�����ִʽ������ҳ����������ļ�
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
			if(strtemp.indexOf(mainbody,1,n_mainbody)>1||strtemp.indexOf(mainbody2,1,n_mainbody2)>1||strtemp.indexOf(mainbody3,1,n_mainbody3)>1)   //��������ǩ����post_content_main����˵������Ҫ��ȡ����Ϣ
				InformationExtract(fp,root,out,divideword_out,dividetitleout);
			else if(strtemp.indexOf(maintext2,1,n_maintext2)>1)      //�������������ģ�������EndText���������������ȡһ������
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
	CharString paragraph;                           //��Ҫ��ǩ�ĳ�ʼ��
	paragraph="<p>";
	CharString endparagraph;
	endparagraph="</p>";
	CharString div;
	div="div";
	CharString enddiv;
	enddiv="</div>";
	char temp;
	//CharString maintext;
	Stack<CharString> s;                            //���ڴ����ǩǶ�׵�ջs
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
			strtemp.addChar('\0');                                           //��ȡһ����ǩ
			tag=strtemp.TagDefine();
			switch(tag)
			{
			case 0:  //ͷ��ǩ
				if(strtemp==paragraph)                                       //ͷ��ǩ��<p>����һ�����ֵĿ�ʼ����ʼ��ȡ����
					PragraphExtract(fp,out,divideword_out,root);
				else if(enddiv.TagCheck(strtemp)==1)                         //ͷ��ǩ��<div...>��˵����Ƕ�ף���Ҫջ���ж�����һ��</div>��������ȡ
					s.push(strtemp);
				break;
			case 1:  //β��ǩ
				if(strtemp==enddiv)
				{
					if(s.empty()!=0)
						s.pop();                                             //��</div>��Ҫ��ջ�е�<div>��ջ
					else
						flag=1;                                              //��ջ�ѿգ�˵�����</div>��ʾ�������Ĳ��ֽ���
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
	CharString endparagraph;                                 //��Ҫ��ǩ��ʼ��
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
	CharString paragraph;                                     //�ö����ݣ������Ҫ�����ӡ���ļ���
	Stack<CharString> s;
	bool open_or_close=0;
	char x;
	int cengshu=0;
	while(true)                                              //��ʼ��ȡ��ÿһ���ַ�Ϊ��ǩ���ǩ�ⲿ������
	{
		bool tag_or_not=0;                                   //0Ϊ��ǩ�����ݣ�1Ϊ��ǩ
		CharString *temp;
		temp=new CharString;
		x=fgetc(fp);
		if(open_or_close==1)
		{
			tag_or_not=1;
			temp->addChar('<');
		}
		open_or_close=0;
		if(x!=' '&&x!='\n'&&x!='	'&&x!='��')               
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
				tag_or_not=1;                   //����һ����ǩ
				break;
			}
			if(tag_or_not==0)
			{
				if(x!=' '&&x!='\n'&&x!='	'&&x!='��')//���˿ո�ͻس�
					temp->addChar(x);
			}
			else
				temp->addChar(x);
		}
		temp->addChar('\0');
		if(tag_or_not==0)                       //�Ǳ�ǩ������
		{
			if(s.empty()==0)
				paragraph.concat(paragraph,*temp);
			else if(endsuperlink.TagCheck(s.elemOnTheTop())==1||s.elemOnTheTop()==font)
				paragraph.concat(paragraph,*temp);
		}
		else                                    //��һ����ǩ������ԭ�򣺳��˱�ʾ�����ӵı�ǩ��������ǩǶ���е�����һ�ɲ����
		{
			int tagtype;
			tagtype=temp->TagDefine();          //�жϱ�ǩ����
			switch(tagtype)
			{
			case 0:                             //ͷ��ǩ
				if(temp->indexOf(endmaintext,1,n_endmaintext)>3) //��ep-source��ǩ��������ʾ�������Ķ�ȡ��ֻʹ��div�жϿ����б�ǩ���պ��������Ϊ���գ�
				{
					if(paragraph.GetLength()>3)                 //���������Σ�˵���п����ǵ������ţ�����˵�����ͬ
					{
						paragraph.PrintString(out);
						divideword(&paragraph,root,divideword_out);
					}
					return fp;
				}
				if(endparagraph.TagCheck(*temp)==1)             //��һ��ͷ��ǩ���ж�Ƕ�ײ���
				{
					cengshu++;
					break;
				}
				if(s.empty()==0)                                //ջΪ����ѹջ
				{
					s.push(*temp);
				}
				else
				{
					if(temp->SameTag(s.elemOnTheTop()))         //�����ջ�б�ǩ��ͬһ�֣���ѹջ��ջ��ֻ����һ�ֱ�ǩ��
						s.push(*temp);
					else
						delete temp;
				}
				break;
			case 1:                                             //��β��ǩ
				if(s.empty()==0)                                //ջΪ�յ����
				{
					if(*temp==enddiv)                           //����</div>�Զ�ֹͣ��ȡ
					{
						delete temp;
						return fp;
					}
					else if(*temp==endparagraph)                //����</p>�ж�Ƕ�ײ���������������ģ���˵���ö������
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
				else                                            //ջ��Ϊ��
				{
					if(temp->TagCheck(s.elemOnTheTop())==1)     //��ջ�б�ǩ��Ӧ��β��ǩ�����ջ
						s.pop();
					else if(*temp==endparagraph)                //��</p>��Ƕ�ײ�����һ
					{
						cengshu--;
						if(cengshu<0)                           //����<p>��Ƕ�ף���˵���ö��������ֱ�����
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