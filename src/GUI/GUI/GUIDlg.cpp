
// GUIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GUI.h"
#include "GUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// CGUIDlg �Ի���



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


// CGUIDlg ��Ϣ�������

BOOL CGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	if(counter==1&&temp[0]=='\0')//�ж������Ƿ�Ϊ��
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
		;//�����޽����ʾ

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CGUIDlg::OnBnClickedaddButton1()
{

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CGUIDlg::OnStnClickedsummondStatic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
