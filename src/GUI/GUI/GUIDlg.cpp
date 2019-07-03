
// GUIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GUI.h"
#include "GUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGUIDlg 对话框



CGUIDlg::CGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGUIDlg::IDD, pParent)
	, m_editsummond(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mdlg=NULL;
	myresult=NULL;
}

void CGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, ID_WIZNEXT, m_editsummond);

}

BEGIN_MESSAGE_MAP(CGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CGUIDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_add_BUTTON1, &CGUIDlg::OnBnClickedaddButton1)
	ON_STN_CLICKED(IDC_summond_STATIC, &CGUIDlg::OnStnClickedsummondStatic)
END_MESSAGE_MAP()


// CGUIDlg 消息处理程序

BOOL CGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGUIDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	char *buffer;
	//a=new char[512];
	buffer=m_editsummond.GetBuffer(m_editsummond.GetLength());

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
	if(counter==1&&temp[0]=='\0')//判断输入是否为空
	{
		mdlg=new cmydialog();
		mdlg->Create(IDD_TIP_DIALOG1,this);
		mdlg->ShowWindow(SW_SHOW);
		return;
	}
	cursor=0;
	FileLinkList **keywordlist;
	keywordlist=new FileLinkList*[counter];
	for(int i=0;i<counter;i++)
		keywordlist[i]=NULL;
	for(i=0;i<counter;i++)
	{
		keywordlist[i]=InvertedFileIndex->Search(list[i]);
	}
	i=0;
	while(!keywordlist[i]&&i<counter)
		i++;
	if(i==counter)
	{
		mdlg=new cmydialog();
		mdlg->Create(IDD_TIP_DIALOG1,this);
		mdlg->ShowWindow(SW_SHOW);
		return;
		//fprintf(fout,"\n");
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
	else
		;//检索无结果提示

	delete[] keywordlist;

	if(NULL==myresult)
	{
		myresult=new search_result();
		myresult->Create(IDD_search_result_DIALOG1,this);
		myresult->get_result_list(tmp);
	}
	myresult->ShowWindow(SW_SHOW);
	myresult=NULL;
	/*
	if(NULL==mdlg)
	{
		mdlg=new cmydialog();
		mdlg->Create(IDD_TIP_DIALOG1,this);
	}
	mdlg->ShowWindow(SW_SHOW);
	*/
	// TODO: 在此添加控件通知处理程序代码
}


void CGUIDlg::OnBnClickedaddButton1()
{

	// TODO: 在此添加控件通知处理程序代码
}

void CGUIDlg::OnStnClickedsummondStatic()
{
	// TODO: 在此添加控件通知处理程序代码
}
