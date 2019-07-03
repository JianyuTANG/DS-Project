#include "search_recommendation.h"
#include <cmath>
#include <iostream>

void bulk_search(AVL_Tree *InvertedFileIndex)
{
	FILE *fp,*fout;
	fp=fopen("query1.txt","r");
	fout=fopen("result1.txt","w");  //输入输出文件
	char buffer[1024];
	while(fgets(buffer,1023,fp))               //按行读取
	{
		CharString list[2000];                 //存储这一行的所有关键词
		int i=0,cursor=0;
		char temp[200];
		int counter=0;
		while(buffer[i]!='\n'&&buffer[i]!='\0')  //遇到空格回车等自动切割
		{
			if(buffer[i]==' ')        //空格分割
			{
				temp[cursor]='\0';
				list[counter]=temp;
				counter++;
				cursor=0;
			}
			else
			{
				temp[cursor]=buffer[i];
				cursor++;
			}
			i++;
		}
		temp[cursor]='\0';
		list[counter]=temp;
		counter++;                                         //counter为关键词数量
		cursor=0;
		FileLinkList **keywordlist;                        //这些关键词对应的文档链表
		keywordlist=new FileLinkList*[counter];
		for(int kk=0;kk<counter;kk++)                    
			keywordlist[kk]=NULL;
		for(i=0;i<counter;i++)
		{
			keywordlist[i]=InvertedFileIndex->Search(list[i]);  //在倒排索引中检索每一个关键词返回对应的文档链表指针
		}
		i=0;
		while(!keywordlist[i]&&i<counter)
			i++;
		if(i==counter)
		{
			fprintf(fout,"\n");                                 //所有文档链表指针为空，即没有与关键词匹配的新闻
			continue;
		}
		FileLinkList *merged_filelinklist;              //归并后的文档链表
		merged_filelinklist=keywordlist[i];             //开始归并
		i++;
		for(;i<counter;i++)                             //依次将文档链表中的节点归并入merged_filelinklist
		{
			linknode *tmp=NULL;
			if(keywordlist[i])
				tmp=keywordlist[i]->head->next;
			while(tmp){
				merged_filelinklist->Add_for_merge(tmp);
				tmp=tmp->next;
			}
		}
		linknode* tmp=NULL;
		if(merged_filelinklist)                       //归并后不为空
			tmp=merged_filelinklist->head;
		if(tmp)
			tmp=tmp->next;
		while(tmp)
		{
			fprintf(fout,"(%d,%d)",tmp->filecode,tmp->num);    //按格式输出
			if(tmp->next)
				fprintf(fout," ");
			else
			{
				fprintf(fout,"\n");
				break;
			}
			tmp=tmp->next;
		}
		delete[] keywordlist;  //！！！！！释放空间，防止堆栈溢出
	}
	printf("succeed\n");
	fclose(fp);
	fclose(fout);
}

void bulk_search2(CharString **title_list,AVL_Tree *InvertedFileIndex,CharString **top_x,int length,FILE *fout)
{
	//用于推荐时的搜索接口，非批量搜索时直接调用
	int i=0;
	FileLinkList **keywordlist;
	keywordlist=new FileLinkList*[length];     //不同关键词的文档链表指针
	for(int kk=0;kk<length;kk++)
		keywordlist[kk]=NULL;
	for(i=0;i<length;i++)
		if(top_x[i])
			keywordlist[i]=InvertedFileIndex->Search(*top_x[i]);  //在倒排索引中检索得到文档链表
	FileLinkList *merged_filelinklist=keywordlist[0];
	for(i=1;i<length;i++)
	{
		linknode *tmp=NULL;
		if(keywordlist[i]->head)
			tmp=keywordlist[i]->head->next;
		while(tmp){
			merged_filelinklist->Add_for_merge(tmp);         //依次归并现有的文档链表
			tmp=tmp->next;
		}
	}
	linknode* tmp=merged_filelinklist->head->next;
	tmp=tmp->next;
	if(!tmp)                                              //归并后的文档链表为空，即无结果，输出相应内容
		fprintf(fout,"该新闻不在数据库中，无法推荐\n");
	int counter=0;
	while(tmp)
	{
		if(title_list[tmp->filecode])
		{
			char *kkk=title_list[tmp->filecode]->GetData();
			fprintf(fout,"(%d,%s)",tmp->filecode,kkk);  //有可能部分网页读不出标题来，可能造成问题
			counter++;
			if(tmp->next&&counter<5)
				fprintf(fout,",");
			else
			{
				fprintf(fout,"\n");
				break;
			}

		}
		tmp=tmp->next;
	}
	delete[] keywordlist;
}

void TF_IDF(int code,AVL_Tree *InvertedFileIndex,CharString &x,CharString **top_three,double *point_top_three,int *word_num,int total_doc_num)
{
	double point=0;                                       //TF-IDF算法，对一个给定词求分值并排序，取前三名
	FileLinkList *temp;
	temp=InvertedFileIndex->Search(x);         //在倒排索引中检索该词
	if(temp==NULL)
		return;          //！！！异常情况返回
	//printf("%d",code);
	linknode *tmp=temp->Search(code);          //在倒排索引中得到的文档链表中搜索这个文档
	if(tmp==NULL)
	{
		return; //防止异常情况
	}
	int num=tmp->num;                         //该词在这个文档中出现次数
	double tf=0,idf=0;
	idf=(double)temp->doc_frequency/(double)total_doc_num;
	idf=-log10(idf);                         //反向文档频率idf的计算
	tf=(double)num/(double)word_num[code];   //词频tf的计算
	point=tf*idf;                            //两值相乘得到该词最终分值
	//std::cout<<point;
	int i=0;
	while(i<=2)                              //一下为维护一个分值为前三名的词汇的优先队列
	{
		if(point<point_top_three[i])
			i++;                             //定位
		else
			break;
	}
	if(i<=2)                                 //这个词处于前三名，注意防止越界等情况
	{
		if(point_top_three[i]==-100000)      //后一个位置还没有人的情况
		{
			top_three[i]=new CharString;
			*top_three[i]=x.GetData();
			point_top_three[i]=point;
		}
		else
		{
			for(int j=2;j>i;j--)
			{
				if(point_top_three[j-1]!=-100000)
				{
					if(point_top_three[j]==-100000)
					{
						point_top_three[j]=point_top_three[j-1];
						top_three[j]=new CharString;            //该位置还没有词，先开辟空间
						*top_three[j]=top_three[j-1]->GetData();
					}
					else
					{
						point_top_three[j]=point_top_three[j-1];
						*top_three[j]=top_three[j-1]->GetData();
					}
				}
			}
			point_top_three[i]=point;
			*top_three[i]=x.GetData();
		}
	}
}

int fuzzy_search(AVL_Tree *InvertedFileIndex,CharString &title,character *root)//对输入的标题进行模糊匹配
{                                                                    //返回匹配的文件编号
	CharStringLink *result; 
	result=divideword2(&title,root);                //先对输入标题分词，返回字符串链表result
	if(result==NULL)
		return -1;       //异常情况处理
	CharStringLinkNode *temp=result->gethead();     //得到字符串链表头节点
	int url[5000];                                  //url[i]即为编号为i的文档中出现了关键词中的多少个
	for(int i=0;i<5000;i++)                         //在这个算法中，只有关键词数量和url[i]匹配时才认为二者时匹配的
		url[i]=0;
	FileLinkList *tmp;
	int max=0,max_pos=0;
	int counter=0;
	int multi_max_num=0;
	int multi_max_pos[100];
	for(int kk=0;kk<100;kk++)
		multi_max_pos[kk]=0;
	while(temp!=NULL)                          //遍历字符串链表，对每个关键词分别匹配
	{
		counter++;
		tmp=InvertedFileIndex->Search(temp->word);  //倒排索引检索得到文档链表
		if(!tmp)
		{
			temp=temp->next;  //异常！！！
			continue;
		}
		linknode *cursor=tmp->head;
		while(cursor!=NULL)
		{
			int k=cursor->filecode;
			//printf("%d ",k);
			if(k>=0)
			{
				url[k]++;                //对应文档的数量加一
				if(url[k]==max)            //维护最大值max
					multi_max_pos[multi_max_num++]=k;
				else if(url[k]>max)
				{
					max=url[k];
					max_pos=k;
					multi_max_num=0;
					for(int kk=0;kk<100;kk++)
						multi_max_pos[kk]=-1;
				}
			}
			cursor=cursor->next;
		}
		temp=temp->next;
	}
	//printf("\n");
	delete result;
	//printf("%d %d\n",max,counter);
	if(max>=counter&&max>=2)          //最终判断max是否于counter相等，这里为安全期间用大于等于
	{                                 //关键词数量较少的情况直接过滤掉
		if(!multi_max_num)
			return max_pos;
		else
		{
			return max_pos;                 //数据库并不大，这种情况发生的概率极小
		}
	}
	else 
		return -1;
}

void bulk_recommendation(CharString **title_list,AVL_Tree *InvertedFileIndex,AVL_Tree *InvertedFileIndex_title,character *root,int *word_num,int total_doc_num)
{
	FILE *fp,*fout;
	fp=fopen("query2.txt","r");
	fout=fopen("result2.txt","w");   //注意不能忘记fclose
	char buffer[1024];
	bool flag=true;
	while(flag)      //按行读取
	{
		flag=fgets(buffer,1023,fp);
		CharString title;
		title=buffer;
		title.remove2();
		int code=fuzzy_search(InvertedFileIndex_title,title,root);  //模糊匹配
		if(code==-1)                                          //匹配失败
			fprintf(fout,"该新闻不在数据库中，无法推荐\n");
		else                                                 //匹配成功
		{
			char *wordfile;
			wordfile=getfilename(code);
			printf("%s\n",wordfile);
			FILE *word;
			if(word=fopen(wordfile,"r"))
				;
			else
			{
				fprintf(fout,"该新闻不在数据库中，无法推荐\n"); //打开文件失败  异常！！！！！
				//printf("error\n");
				//delete wordfile;
				continue;
			}
			char buf[256];
			CharString *top_three[3];                  //分值最大的三个词汇的优先队列
			for(int kkk=0;kkk<3;kkk++)
				top_three[kkk]=NULL;
			double point_top_three[3];
			for(int i=0;i<3;i++)
				point_top_three[i]=-100000;            //这三个词的分值，初始化为一个较小的数
			CharStringLink existed_words;
			while(fgets(buf,255,word))                //按行读取
			{
				CharString tmp;
				tmp=buf;
				tmp.remove2();
				//printf("666\n");
				if(existed_words.search(tmp)==NULL)
				{
					existed_words.add(tmp);
					TF_IDF(code,InvertedFileIndex,tmp,top_three,point_top_three,word_num,total_doc_num);
				}                                     //计算分值
			}
			fclose(word);
			if(top_three[0])                      //得到了三个关键词的情况，直接检索
				bulk_search2(title_list,InvertedFileIndex,top_three,3,fout);
			else
				fprintf(fout,"该新闻不在数据库中，无法推荐\n");//无对应关键词  异常！！！！！！！
			delete wordfile;
		}
	}
	fclose(fp);
	fclose(fout);
}