// cmydialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GUI.h"
#include "cmydialog.h"
#include "afxdialogex.h"


// cmydialog 对话框

IMPLEMENT_DYNAMIC(cmydialog, CDialogEx)

cmydialog::cmydialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(cmydialog::IDD, pParent)
{

}

cmydialog::~cmydialog()
{
}

void cmydialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cmydialog, CDialogEx)
END_MESSAGE_MAP()


// cmydialog 消息处理程序
