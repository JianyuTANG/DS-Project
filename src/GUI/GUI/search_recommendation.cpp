
#include "stdafx.h"
#include <cmath>

void bulk_search(AVL_Tree *InvertedFileIndex)
{
	FILE *fp,*fout;
	fopen_s(&fp,"query1.txt","r");
	fopen_s(&fout,"result1.txt","w");
	char buffer[1024];
	while(fgets(buffer,1023,fp))
	{
		CharString list[2000];
		int i=0,cursor=0;
		char temp[200];
		int counter=0;
		while(buffer[i]!='\n'&&buffer[i]!='\0')
		{
			if(buffer[i]==' ')
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
		counter++;
		cursor=0;
		FileLinkList **keywordlist;
		keywordlist=new FileLinkList*[counter];
		memset(keywordlist,NULL,counter);
		for(i=0;i<counter;i++)
		{
			keywordlist[i]=InvertedFileIndex->Search(list[i]);
		}
		i=0;
		while(!keywordlist[i]&&i<counter)
			i++;
		if(i==counter)
		{
			fprintf(fout,"\n");
			continue;
		}
		FileLinkList *merged_filelinklist;
		merged_filelinklist=keywordlist[i];
		i++;
		for(;i<counter;i++)
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
		if(merged_filelinklist)
			tmp=merged_filelinklist->head;
		if(tmp)
			tmp=tmp->next;
		while(tmp)
		{
			fprintf(fout,"(%d,%d)",tmp->filecode,tmp->num);
			if(tmp->next)
				fprintf(fout," ");
			else
			{
				fprintf(fout,"\n");
				break;
			}
			tmp=tmp->next;
		}
		delete[] keywordlist;
	}
	printf("succeed\n");
	fclose(fp);
	fclose(fout);
}

void bulk_search2(CharString **title_list,AVL_Tree *InvertedFileIndex,CharString **top_x,int length,FILE *fout)
{
	//printf("6666666666\n\n");
	int i=0;
	FileLinkList **keywordlist;
	keywordlist=new FileLinkList*[length];
	memset(keywordlist,NULL,length);
	for(i=0;i<length;i++)
		if(top_x[i])
			keywordlist[i]=InvertedFileIndex->Search(*top_x[i]);
	FileLinkList *merged_filelinklist=keywordlist[0];
	for(i=1;i<length;i++)
	{
		linknode *tmp=NULL;
		if(keywordlist[i]->head)
			tmp=keywordlist[i]->head->next;
		while(tmp){
			merged_filelinklist->Add_for_merge(tmp);
			tmp=tmp->next;
		}
	}
	linknode* tmp=merged_filelinklist->head->next;
	tmp=tmp->next;
	if(!tmp)
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

void TF_IDF(int code,CharString &x,CharString **top_three,double *point_top_three)
{
	double point=0;
	FileLinkList *temp;
	temp=InvertedFileIndex->Search(x);
	if(temp==NULL)
		return;
	//printf("%d",code);
	linknode *tmp=temp->Search(code);
	if(tmp==NULL)
	{
		//printf("6667");//666666666666666666666666666666666666
		return;
	}
	int num=tmp->num;
	double tf=0,idf=0;
	idf=(double)temp->doc_frequency/(double)total_doc_num;
	idf=-log10(idf);
	tf=(double)num/(double)word_num[code];
	point=tf*idf;
	//std::cout<<point;
	int i=0;
	while(i<=2)
	{
		if(point<point_top_three[i])
			i++;
		else
			break;
	}
	if(i<=2)
	{
		if(point_top_three[i]==-100000)
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
						top_three[j]=new CharString;
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
	/*
	while(i<2)
	{
	if(point<point_top_three[i])
	i++;
	else if(point_top_three[i]==-100000)
	{
	*top_three[i]=x;
	point_top_three[i]=point;
	break;
	}
	else
	{
	for(int j=2;j>i;j--)
	{
	if(point_top_three[j-1]!=-100000)
	{
	point_top_three[j]=point_top_three[j-1];
	*top_three[j]=*top_three[j-1];
	}
	}
	point_top_three[i]=point;
	*top_three[i]=x;
	break;
	}
	}
	*/
}

int fuzzy_search(AVL_Tree *InvertedFileIndex,CharString &title,character *root)
{
	CharStringLink *result;
	result=divideword2(&title,root);
	if(result==NULL)
		return -1;
	CharStringLinkNode *temp=result->gethead();
	int url[5000];
	memset(url,0,5000);
	FileLinkList *tmp;
	int max=0,max_pos=0;
	int counter=0;
	int multi_max_num=0;
	int multi_max_pos[100];
	memset(multi_max_pos,0,100);
	while(temp!=NULL)
	{
		counter++;
		//if(!InvertedFileIndex)
		//	printf("99999999999");
		tmp=InvertedFileIndex->Search(temp->word);
		if(!tmp)
		{
			temp=temp->next;
			continue;
		}
		linknode *cursor=tmp->head;
		while(cursor!=NULL)
		{
			int k=cursor->filecode;
			//printf("%d ",k);
			if(k>=0)
			{
				url[k]++;
				if(url[k]==max)
					multi_max_pos[multi_max_num++]=k;
				else if(url[k]>max)
				{
					max=url[k];
					max_pos=k;
					multi_max_num=0;
					memset(multi_max_pos,-1,100);
				}
			}
			cursor=cursor->next;
		}
		temp=temp->next;
	}
	printf("\n");
	delete result;
	printf("%d %d\n",max,counter);
	if(max>=counter&&max>=2)
	{
		if(!multi_max_num)
			return max_pos;
		else
		{
			multi_max_pos[multi_max_num]=max_pos;
			for(int j=0;j<=multi_max_num;j++)
				printf("997799\n");//99999999999999999999999999999999999999999999999999999999999999
		}
	}
	else 
		return -1;
}
/*
void bulk_recommendation(CharString **title_list,AVL_Tree *InvertedFileIndex,AVL_Tree *InvertedFileIndex_title,character *root,int *word_num,int total_doc_num)
{
	FILE *fp,*fout;
	fopen_s(&fp,"query1.txt","r");
	fopen_s(&fout,"result1.txt","w");
	char buffer[1024];
	while(fgets(buffer,1023,fp))
	{
		CharString title;
		title=buffer;
		title.remove2();
		int code=fuzzy_search(InvertedFileIndex_title,title,root);
		if(code==-1)
			fprintf(fout,"该新闻不在数据库中，无法推荐\n");
		else
		{
			char *wordfile;
			wordfile=getfilename(code);
			printf("%s\n",wordfile);
			FILE *word;
			if(fopen_s(&word,wordfile,"r")==0)
				;
			else
			{
				fprintf(fout,"该新闻不在数据库中，无法推荐\n");
				//printf("error\n");
				delete wordfile;
				continue;
			}
			char buf[256];
			CharString *top_three[3];
			memset(top_three,NULL,3);
			double point_top_three[3];
			for(int i=0;i<3;i++)
				point_top_three[i]=-100000;
			CharStringLink existed_words;
			while(fgets(buf,255,word))
			{
				CharString tmp;
				tmp=buf;
				tmp.remove2();
				//printf("666\n");
				if(existed_words.search(tmp)==NULL)
				{
					existed_words.add(tmp);
					TF_IDF(code,InvertedFileIndex,tmp,top_three,point_top_three,word_num,total_doc_num);
				}
			}
			fclose(word);
			if(top_three[0])
				bulk_search2(title_list,InvertedFileIndex,top_three,3,fout);
			else
				fprintf(fout,"该新闻不在数据库中，无法推荐\n");
			delete wordfile;
		}
	}
	fclose(fp);
	fclose(fout);
}
*/