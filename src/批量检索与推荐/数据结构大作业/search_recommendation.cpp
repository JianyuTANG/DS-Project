#include "search_recommendation.h"
#include <cmath>
#include <iostream>

void bulk_search(AVL_Tree *InvertedFileIndex)
{
	FILE *fp,*fout;
	fp=fopen("query1.txt","r");
	fout=fopen("result1.txt","w");  //��������ļ�
	char buffer[1024];
	while(fgets(buffer,1023,fp))               //���ж�ȡ
	{
		CharString list[2000];                 //�洢��һ�е����йؼ���
		int i=0,cursor=0;
		char temp[200];
		int counter=0;
		while(buffer[i]!='\n'&&buffer[i]!='\0')  //�����ո�س����Զ��и�
		{
			if(buffer[i]==' ')        //�ո�ָ�
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
		counter++;                                         //counterΪ�ؼ�������
		cursor=0;
		FileLinkList **keywordlist;                        //��Щ�ؼ��ʶ�Ӧ���ĵ�����
		keywordlist=new FileLinkList*[counter];
		for(int kk=0;kk<counter;kk++)                    
			keywordlist[kk]=NULL;
		for(i=0;i<counter;i++)
		{
			keywordlist[i]=InvertedFileIndex->Search(list[i]);  //�ڵ��������м���ÿһ���ؼ��ʷ��ض�Ӧ���ĵ�����ָ��
		}
		i=0;
		while(!keywordlist[i]&&i<counter)
			i++;
		if(i==counter)
		{
			fprintf(fout,"\n");                                 //�����ĵ�����ָ��Ϊ�գ���û����ؼ���ƥ�������
			continue;
		}
		FileLinkList *merged_filelinklist;              //�鲢����ĵ�����
		merged_filelinklist=keywordlist[i];             //��ʼ�鲢
		i++;
		for(;i<counter;i++)                             //���ν��ĵ������еĽڵ�鲢��merged_filelinklist
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
		if(merged_filelinklist)                       //�鲢��Ϊ��
			tmp=merged_filelinklist->head;
		if(tmp)
			tmp=tmp->next;
		while(tmp)
		{
			fprintf(fout,"(%d,%d)",tmp->filecode,tmp->num);    //����ʽ���
			if(tmp->next)
				fprintf(fout," ");
			else
			{
				fprintf(fout,"\n");
				break;
			}
			tmp=tmp->next;
		}
		delete[] keywordlist;  //�����������ͷſռ䣬��ֹ��ջ���
	}
	printf("succeed\n");
	fclose(fp);
	fclose(fout);
}

void bulk_search2(CharString **title_list,AVL_Tree *InvertedFileIndex,CharString **top_x,int length,FILE *fout)
{
	//�����Ƽ�ʱ�������ӿڣ�����������ʱֱ�ӵ���
	int i=0;
	FileLinkList **keywordlist;
	keywordlist=new FileLinkList*[length];     //��ͬ�ؼ��ʵ��ĵ�����ָ��
	for(int kk=0;kk<length;kk++)
		keywordlist[kk]=NULL;
	for(i=0;i<length;i++)
		if(top_x[i])
			keywordlist[i]=InvertedFileIndex->Search(*top_x[i]);  //�ڵ��������м����õ��ĵ�����
	FileLinkList *merged_filelinklist=keywordlist[0];
	for(i=1;i<length;i++)
	{
		linknode *tmp=NULL;
		if(keywordlist[i]->head)
			tmp=keywordlist[i]->head->next;
		while(tmp){
			merged_filelinklist->Add_for_merge(tmp);         //���ι鲢���е��ĵ�����
			tmp=tmp->next;
		}
	}
	linknode* tmp=merged_filelinklist->head->next;
	tmp=tmp->next;
	if(!tmp)                                              //�鲢����ĵ�����Ϊ�գ����޽���������Ӧ����
		fprintf(fout,"�����Ų������ݿ��У��޷��Ƽ�\n");
	int counter=0;
	while(tmp)
	{
		if(title_list[tmp->filecode])
		{
			char *kkk=title_list[tmp->filecode]->GetData();
			fprintf(fout,"(%d,%s)",tmp->filecode,kkk);  //�п��ܲ�����ҳ�������������������������
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
	double point=0;                                       //TF-IDF�㷨����һ�����������ֵ������ȡǰ����
	FileLinkList *temp;
	temp=InvertedFileIndex->Search(x);         //�ڵ��������м����ô�
	if(temp==NULL)
		return;          //�������쳣�������
	//printf("%d",code);
	linknode *tmp=temp->Search(code);          //�ڵ��������еõ����ĵ���������������ĵ�
	if(tmp==NULL)
	{
		return; //��ֹ�쳣���
	}
	int num=tmp->num;                         //�ô�������ĵ��г��ִ���
	double tf=0,idf=0;
	idf=(double)temp->doc_frequency/(double)total_doc_num;
	idf=-log10(idf);                         //�����ĵ�Ƶ��idf�ļ���
	tf=(double)num/(double)word_num[code];   //��Ƶtf�ļ���
	point=tf*idf;                            //��ֵ��˵õ��ô����շ�ֵ
	//std::cout<<point;
	int i=0;
	while(i<=2)                              //һ��Ϊά��һ����ֵΪǰ�����Ĵʻ�����ȶ���
	{
		if(point<point_top_three[i])
			i++;                             //��λ
		else
			break;
	}
	if(i<=2)                                 //����ʴ���ǰ������ע���ֹԽ������
	{
		if(point_top_three[i]==-100000)      //��һ��λ�û�û���˵����
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
						top_three[j]=new CharString;            //��λ�û�û�дʣ��ȿ��ٿռ�
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

int fuzzy_search(AVL_Tree *InvertedFileIndex,CharString &title,character *root)//������ı������ģ��ƥ��
{                                                                    //����ƥ����ļ����
	CharStringLink *result; 
	result=divideword2(&title,root);                //�ȶ��������ִʣ������ַ�������result
	if(result==NULL)
		return -1;       //�쳣�������
	CharStringLinkNode *temp=result->gethead();     //�õ��ַ�������ͷ�ڵ�
	int url[5000];                                  //url[i]��Ϊ���Ϊi���ĵ��г����˹ؼ����еĶ��ٸ�
	for(int i=0;i<5000;i++)                         //������㷨�У�ֻ�йؼ���������url[i]ƥ��ʱ����Ϊ����ʱƥ���
		url[i]=0;
	FileLinkList *tmp;
	int max=0,max_pos=0;
	int counter=0;
	int multi_max_num=0;
	int multi_max_pos[100];
	for(int kk=0;kk<100;kk++)
		multi_max_pos[kk]=0;
	while(temp!=NULL)                          //�����ַ���������ÿ���ؼ��ʷֱ�ƥ��
	{
		counter++;
		tmp=InvertedFileIndex->Search(temp->word);  //�������������õ��ĵ�����
		if(!tmp)
		{
			temp=temp->next;  //�쳣������
			continue;
		}
		linknode *cursor=tmp->head;
		while(cursor!=NULL)
		{
			int k=cursor->filecode;
			//printf("%d ",k);
			if(k>=0)
			{
				url[k]++;                //��Ӧ�ĵ���������һ
				if(url[k]==max)            //ά�����ֵmax
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
	if(max>=counter&&max>=2)          //�����ж�max�Ƿ���counter��ȣ�����Ϊ��ȫ�ڼ��ô��ڵ���
	{                                 //�ؼ����������ٵ����ֱ�ӹ��˵�
		if(!multi_max_num)
			return max_pos;
		else
		{
			return max_pos;                 //���ݿⲢ����������������ĸ��ʼ�С
		}
	}
	else 
		return -1;
}

void bulk_recommendation(CharString **title_list,AVL_Tree *InvertedFileIndex,AVL_Tree *InvertedFileIndex_title,character *root,int *word_num,int total_doc_num)
{
	FILE *fp,*fout;
	fp=fopen("query2.txt","r");
	fout=fopen("result2.txt","w");   //ע�ⲻ������fclose
	char buffer[1024];
	bool flag=true;
	while(flag)      //���ж�ȡ
	{
		flag=fgets(buffer,1023,fp);
		CharString title;
		title=buffer;
		title.remove2();
		int code=fuzzy_search(InvertedFileIndex_title,title,root);  //ģ��ƥ��
		if(code==-1)                                          //ƥ��ʧ��
			fprintf(fout,"�����Ų������ݿ��У��޷��Ƽ�\n");
		else                                                 //ƥ��ɹ�
		{
			char *wordfile;
			wordfile=getfilename(code);
			printf("%s\n",wordfile);
			FILE *word;
			if(word=fopen(wordfile,"r"))
				;
			else
			{
				fprintf(fout,"�����Ų������ݿ��У��޷��Ƽ�\n"); //���ļ�ʧ��  �쳣����������
				//printf("error\n");
				//delete wordfile;
				continue;
			}
			char buf[256];
			CharString *top_three[3];                  //��ֵ���������ʻ�����ȶ���
			for(int kkk=0;kkk<3;kkk++)
				top_three[kkk]=NULL;
			double point_top_three[3];
			for(int i=0;i<3;i++)
				point_top_three[i]=-100000;            //�������ʵķ�ֵ����ʼ��Ϊһ����С����
			CharStringLink existed_words;
			while(fgets(buf,255,word))                //���ж�ȡ
			{
				CharString tmp;
				tmp=buf;
				tmp.remove2();
				//printf("666\n");
				if(existed_words.search(tmp)==NULL)
				{
					existed_words.add(tmp);
					TF_IDF(code,InvertedFileIndex,tmp,top_three,point_top_three,word_num,total_doc_num);
				}                                     //�����ֵ
			}
			fclose(word);
			if(top_three[0])                      //�õ��������ؼ��ʵ������ֱ�Ӽ���
				bulk_search2(title_list,InvertedFileIndex,top_three,3,fout);
			else
				fprintf(fout,"�����Ų������ݿ��У��޷��Ƽ�\n");//�޶�Ӧ�ؼ���  �쳣��������������
			delete wordfile;
		}
	}
	fclose(fp);
	fclose(fout);
}