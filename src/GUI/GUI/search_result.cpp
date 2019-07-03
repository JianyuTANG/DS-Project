// search_result.cpp : 实现文件
//

#include "stdafx.h"
#include "GUI.h"
#include "search_result.h"
#include "afxdialogex.h"


// search_result 对话框

IMPLEMENT_DYNAMIC(search_result, CDialogEx)

	search_result::search_result(CWnd* pParent /*=NULL*/)
	: CDialogEx(search_result::IDD, pParent)
	, result1(_T(""))
	, reslut2(_T(""))
	, result3(_T(""))
	, result4(_T(""))
	, result5(_T(""))
{
	for(int i=0;i<=5;i++)
		filecode[i]=-1;
	detail_1=NULL;
}

search_result::~search_result()
{
}

void search_result::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, result1);
	DDX_Text(pDX, IDC_EDIT2, reslut2);
	DDX_Text(pDX, IDC_EDIT3, result3);
	DDX_Text(pDX, IDC_EDIT4, result4);
	DDX_Text(pDX, IDC_EDIT5, result5);
}


BEGIN_MESSAGE_MAP(search_result, CDialogEx)
	ON_BN_CLICKED(IDC_detail_1, &search_result::OnBnClickeddetail1)
	ON_BN_CLICKED(IDC_detail_2, &search_result::OnBnClickeddetail2)
	ON_BN_CLICKED(IDC_detail_3, &search_result::OnBnClickeddetail3)
	ON_BN_CLICKED(IDC_detail_4, &search_result::OnBnClickeddetail4)
	ON_BN_CLICKED(IDC_detail_5, &search_result::OnBnClickeddetail5)
END_MESSAGE_MAP()


// search_result 消息处理程序

void search_result::get_result_list(linknode* tmp)
{
	int counter=0;
	char notfound[]="没有找到更多结果了";
	while(tmp)
	{
		counter++;
		if(counter>5)
			break;
		filecode[counter]=tmp->filecode;
		if(title_list[tmp->filecode])
			switch(counter)
		{
			case 1:
				result1=title_list[tmp->filecode]->GetData();
				break;
			case 2:
				reslut2=title_list[tmp->filecode]->GetData();
				break;
			case 3:
				result3=title_list[tmp->filecode]->GetData();
				break;
			case 4:
				result4=title_list[tmp->filecode]->GetData();
				break;
			case 5:
				result5=title_list[tmp->filecode]->GetData();
				break;
		}
		tmp=tmp->next;
	}
	if(counter<5)
	{
		switch(counter)
		{
		case 0:
			result1=notfound;
			reslut2=notfound;
			result3=notfound;
			result4=notfound;
			result5=notfound;
			break;
		case 1:
			reslut2=notfound;
			result3=notfound;
			result4=notfound;
			result5=notfound;
			break;
		case 2:
			result3=notfound;
			result4=notfound;
			result5=notfound;
			break;
		case 3:
			result4=notfound;
			result5=notfound;
			break;
		case 4:
			result5=notfound;
			break;
		}
	}
	UpdateData(FALSE);
}

void search_result::OnBnClickeddetail1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(filecode[1]==-1)
		return;
	if(NULL==detail_1)
	{
		detail_1=new detail_recommendation();
		detail_1->Create(IDD_detail_DIALOG1,this);
		detail_1->recommendation(filecode[1]);
		//detail_1->cannotfindmore();
	}
	detail_1->ShowWindow(SW_SHOW);
	detail_1=NULL;
}


void search_result::OnBnClickeddetail2()
{
	if(filecode[2]==-1)
		return;
	if(NULL==detail_1)
	{
		detail_1=new detail_recommendation();
		detail_1->Create(IDD_detail_DIALOG1,this);
		detail_1->recommendation(filecode[2]);
	}
	detail_1->ShowWindow(SW_SHOW);
	detail_1=NULL;
	// TODO: 在此添加控件通知处理程序代码
}


void search_result::OnBnClickeddetail3()
{
	if(filecode[3]==-1)
		return;
	if(NULL==detail_1)
	{
		detail_1=new detail_recommendation();
		detail_1->Create(IDD_detail_DIALOG1,this);
		detail_1->recommendation(filecode[3]);
	}
	detail_1->ShowWindow(SW_SHOW);
	detail_1=NULL;
	// TODO: 在此添加控件通知处理程序代码
}


void search_result::OnBnClickeddetail4()
{
	if(filecode[4]==-1)
		return;
	if(NULL==detail_1)
	{
		detail_1=new detail_recommendation();
		detail_1->Create(IDD_detail_DIALOG1,this);
		detail_1->recommendation(filecode[4]);
	}
	detail_1->ShowWindow(SW_SHOW);
	detail_1=NULL;
	// TODO: 在此添加控件通知处理程序代码
}


void search_result::OnBnClickeddetail5()
{
	if(filecode[5]==-1)
		return;
	if(NULL==detail_1)
	{
		detail_1=new detail_recommendation();
		detail_1->Create(IDD_detail_DIALOG1,this);
		detail_1->recommendation(filecode[5]);
	}
	detail_1->ShowWindow(SW_SHOW);
	detail_1=NULL;
	// TODO: 在此添加控件通知处理程序代码
}
