
// GUI.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "GUI.h"
#include "GUIDlg.h"
#include "stdio.h"
#include "stdlib.h"
#include "io.h"
#include  "CharString.h"
#include "HashTable.h"
#include "AVL_Tree.h"
#include "search_recommendation.h"
#include "CharStringLink.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGUIApp

BEGIN_MESSAGE_MAP(CGUIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGUIApp 构造

CGUIApp::CGUIApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CGUIApp 对象

CGUIApp theApp;

AVL_Tree *InvertedFileIndex;
AVL_Tree *InvertedFileIndex_title;
CharString **title_list;
int *word_num;
int total_doc_num;
// CGUIApp 初始化

BOOL CGUIApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));


	char *filename;
	filename="词库.dic";                                       
	FILE *fp;
	if(fopen_s(&fp,filename,"r")==0)                                  //打开词库
		;
	else
	{
		//printf("cannot open the dictionary\n");
		return 0;
	}
	character *dictionaryroot;                                  //指向词典树的指针
	//dictionaryroot=initdictionary(fp);                          //加载词库
	InvertedFileIndex=new AVL_Tree;
	InvertedFileIndex_title=new AVL_Tree;
	word_num=new int[5000];
	total_doc_num=781;
	title_list=new CharString*[5000];
	memset(title_list,0,50);
	for(int i=0;i<5000;i++)
		title_list[i]=NULL;
	for(int i=0;i<5000;i++)
		word_num[i]=-1;
	CharString head,file;
	const char *to_search="input\\*.txt";                      //在input文件夹下搜索html文件
	long handle;                                                //用于查找的句柄
	struct _finddata_t fileinfo;                                //存储文件信息的结构体
	handle=_findfirst(to_search,&fileinfo);                     //第一次查找
	if(handle==-1)
		return -1;
	                              //打印出找到的文件的文件名
	head="input\\";
	file=fileinfo.name;
	head.concat(head,file);   
	int urlcode;
	FILE *fp2;
	char buffer[512];
	urlcode=geturlcode(&head);
	if(title_or_word(&head))
	{
		if((fopen_s(&fp2,head.GetData(),"r"))==0)
		{
			//printf("%s\n",fileinfo.name); 
			if(fgets(buffer,511,fp2))
			{
				title_list[urlcode]=new CharString;
				*title_list[urlcode]=buffer;
				title_list[urlcode]->remove2();
			}
			init_InvertedFileIndex(InvertedFileIndex_title,fp2,urlcode,word_num);
			fclose(fp2);
		}
	}
	else
	{
		if((fopen_s(&fp2,head.GetData(),"r"))==0)
		{
			init_InvertedFileIndex(InvertedFileIndex,fp2,urlcode,word_num);
			fclose(fp2);
		}
	}
	//tagdistinguish2(head.GetData(),dictionaryroot);              //解析第一个网页
	while(!_findnext(handle,&fileinfo))                         //查找其他符合的文件
	{
		head="input\\";
		file=fileinfo.name;
		head.concat(head,file);
		
		urlcode=geturlcode(&head);
		if(title_or_word(&head))
		{
			if((fopen_s(&fp2,head.GetData(),"r"))==0)
			{
				//printf("%s\n",fileinfo.name);
				if(fgets(buffer,511,fp2))
				{
					title_list[urlcode]=new CharString;
					*title_list[urlcode]=buffer;
					title_list[urlcode]->remove2();
				}
				init_InvertedFileIndex(InvertedFileIndex_title,fp2,urlcode,word_num);
				fclose(fp2);
			}
		}
		else
		{
			if((fopen_s(&fp2,head.GetData(),"r"))==0)
			{
				init_InvertedFileIndex(InvertedFileIndex,fp2,urlcode,word_num);
				fclose(fp2);
			}
		}
		//tagdistinguish2(head.GetData(),dictionaryroot);          //解析该网页
	}
	_findclose(handle);    
	//bulk_search(InvertedFileIndex);
	//bulk_recommendation(title_list,InvertedFileIndex,InvertedFileIndex_title,dictionaryroot,word_num,781);
	//system("pause");



	CGUIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

