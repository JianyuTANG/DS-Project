// detail_recommendation.cpp : 实现文件
//

#include "stdafx.h"
#include "GUI.h"
#include "detail_recommendation.h"
#include "afxdialogex.h"


// detail_recommendation 对话框

IMPLEMENT_DYNAMIC(detail_recommendation, CDialogEx)

	detail_recommendation::detail_recommendation(CWnd* pParent /*=NULL*/)
	: CDialogEx(detail_recommendation::IDD, pParent)
	, recommendation1(_T(""))
	, recommendation2(_T(""))
	, recommendation3(_T(""))
	, recommendation4(_T(""))
	, recommendation5(_T(""))
	, maintext(_T(""))
{
	m_next2=NULL;
	m_next1=NULL;
	m_next3=NULL;
	m_next4=NULL;
	m_next5=NULL;
	for(int i=0;i<=5;i++)
		filecode[i]=-1;
}

detail_recommendation::~detail_recommendation()
{
}

void detail_recommendation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, recommendation1);
	DDX_Text(pDX, IDC_EDIT3, recommendation2);
	DDX_Text(pDX, IDC_EDIT4, recommendation3);
	DDX_Text(pDX, IDC_EDIT5, recommendation4);
	DDX_Text(pDX, IDC_EDIT6, recommendation5);
	DDX_Text(pDX, IDC_EDIT1, maintext);
	//DDX_Text(pDX, IDC_EDIT7, l);
}


BEGIN_MESSAGE_MAP(detail_recommendation, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &detail_recommendation::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &detail_recommendation::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &detail_recommendation::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &detail_recommendation::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &detail_recommendation::OnBnClickedButton5)
END_MESSAGE_MAP()


// detail_recommendation 消息处理程序
void detail_recommendation::display(int urlcode)
{
	char *filename;
	filename=getfilename_info(urlcode);
	//maintext=filename;
	char *buffer=new char[100000];
	FILE *fin;
	if(fopen_s(&fin,filename,"r")==0)
		;
	else
	{
		maintext="打开文件失败";
	}
	int i=0;
	int counter=0;
	while(!feof(fin))
	{
		buffer[i]=fgetc(fin);
		if(buffer[i]=='\n')
		{
			buffer[i]='\r';
			i++;
			buffer[i]='\n';
		}
		i++;
	}
	buffer[i]='\0';
	maintext=buffer;
	UpdateData(FALSE);
	fclose(fin);
}

void detail_recommendation::recommendation(int urlcode)
{
	char *filename;
	filename=getfilename_info(urlcode);
	//maintext=filename;
	char *buffer=new char[100000];
	FILE *fin;
	if(fopen_s(&fin,filename,"r")==0)
		;
	else
	{
		maintext="打开文件失败";
	}
	int i=0;
	int counter=0;
	while(!feof(fin))
	{
		buffer[i]=fgetc(fin);
		if(buffer[i]=='\n')
		{
			buffer[i]='\r';
			i++;
			buffer[i]='\n';
		}
		i++;
	}
	buffer[i]='\0';
	maintext=buffer;
	UpdateData(FALSE);
	return;
	char notfound[]="没有更多推荐内容了";
	int code=urlcode;
	char *wordfile;
	wordfile=getfilename(code);
	FILE *word;
	if(fopen_s(&word,wordfile,"r")==0)
		;
	else
	{
		cannotfindmore();
		delete wordfile;
	}
	char buf[256];
	CharString *top_three[3];
	for(int i=0;i<3;i++)
		top_three[i]=NULL;
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
			TF_IDF(code,tmp,top_three,point_top_three);
		}
	}
	fclose(word);
	if(top_three[0])
	{
		int i=0;
		FileLinkList **keywordlist;
		keywordlist=new FileLinkList*[3];
		for(int i=0;i<3;i++)
			keywordlist[i]=NULL;
		for(i=0;i<3;i++)
			if(top_three[i])
				keywordlist[i]=InvertedFileIndex->Search(*top_three[i]);
		FileLinkList *merged_filelinklist=keywordlist[0];
		for(i=1;i<3;i++)
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
			cannotfindmore();//无推荐结果的情况
		int counter=0;
		while(tmp)
		{
			if(title_list[tmp->filecode])
			{
				char *kkk=title_list[tmp->filecode]->GetData();
				counter++;
				filecode[counter]=tmp->filecode;
				switch(counter)
				{
				case 1:
					recommendation1=kkk;
					break;
				case 2:
					recommendation2=kkk;
					break;
				case 3:
					recommendation3=kkk;
					break;
				case 4:
					recommendation4=kkk;
					break;
				case 5:
					recommendation5=kkk;
					break;
				}

			}
			tmp=tmp->next;
		}
		if(counter<5)
		{
			switch(counter)
			{
			case 0:
				cannotfindmore();
				break;
			case 1:
				recommendation2=notfound;
				recommendation3=notfound;
				recommendation4=notfound;
				recommendation5=notfound;
				break;
			case 2:
				recommendation3=notfound;
				recommendation4=notfound;
				recommendation5=notfound;
				break;
			case 3:
				recommendation4=notfound;
				recommendation5=notfound;
				break;
			case 4:
				recommendation5=notfound;
				break;
			}
		}
		delete[] keywordlist;

	}
	else
	{
		cannotfindmore();//异常
	}
	UpdateData(FALSE);
}

void detail_recommendation::cannotfindmore()
{
	char notfound[]="没有更多推荐内容了";
	recommendation1=notfound;
	recommendation2=notfound;
	recommendation3=notfound;
	recommendation4=notfound;
	recommendation5=notfound;
	UpdateData(FALSE);
}

void detail_recommendation::OnBnClickedButton1()
{
	if(filecode[1]==-1)
		return;
	if(NULL==m_next1)
	{
		m_next1=new detail_recommendation();
		m_next1->Create(IDD_detail_DIALOG1,this);
		m_next1->recommendation(filecode[1]);
	}
	m_next1->ShowWindow(SW_SHOW);
	// TODO: 在此添加控件通知处理程序代码
}


void detail_recommendation::OnBnClickedButton2()
{
	if(filecode[2]==-1)
		return;
	if(NULL==m_next2)
	{
		m_next2=new detail_recommendation();
		m_next2->Create(IDD_detail_DIALOG1,this);
		m_next2->recommendation(filecode[2]);
	}
	m_next2->ShowWindow(SW_SHOW);
	// TODO: 在此添加控件通知处理程序代码
}


void detail_recommendation::OnBnClickedButton3()
{
	if(filecode[3]==-1)
		return;
	if(NULL==m_next3)
	{
		m_next3=new detail_recommendation();
		m_next3->Create(IDD_detail_DIALOG1,this);
		m_next3->recommendation(filecode[3]);
	}
	m_next3->ShowWindow(SW_SHOW);
	// TODO: 在此添加控件通知处理程序代码
}


void detail_recommendation::OnBnClickedButton4()
{
	if(filecode[4]==-1)
		return;
	if(NULL==m_next4)
	{
		m_next4=new detail_recommendation();
		m_next4->Create(IDD_detail_DIALOG1,this);
		m_next4->recommendation(filecode[4]);
	}
	m_next4->ShowWindow(SW_SHOW);
	// TODO: 在此添加控件通知处理程序代码
}


void detail_recommendation::OnBnClickedButton5()
{
	if(filecode[5]==-1)
		return;
	if(NULL==m_next5)
	{
		m_next5=new detail_recommendation();
		m_next5->Create(IDD_detail_DIALOG1,this);
		m_next5->recommendation(filecode[5]);
	}
	m_next5->ShowWindow(SW_SHOW);
	// TODO: 在此添加控件通知处理程序代码
}
