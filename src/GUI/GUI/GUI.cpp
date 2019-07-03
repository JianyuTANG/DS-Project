
// GUI.cpp : ����Ӧ�ó��������Ϊ��
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


// CGUIApp ����

CGUIApp::CGUIApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CGUIApp ����

CGUIApp theApp;

AVL_Tree *InvertedFileIndex;
AVL_Tree *InvertedFileIndex_title;
CharString **title_list;
int *word_num;
int total_doc_num;
// CGUIApp ��ʼ��

BOOL CGUIApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	char *filename;
	filename="�ʿ�.dic";                                       
	FILE *fp;
	if(fopen_s(&fp,filename,"r")==0)                                  //�򿪴ʿ�
		;
	else
	{
		//printf("cannot open the dictionary\n");
		return 0;
	}
	character *dictionaryroot;                                  //ָ��ʵ�����ָ��
	//dictionaryroot=initdictionary(fp);                          //���شʿ�
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
	const char *to_search="input\\*.txt";                      //��input�ļ���������html�ļ�
	long handle;                                                //���ڲ��ҵľ��
	struct _finddata_t fileinfo;                                //�洢�ļ���Ϣ�Ľṹ��
	handle=_findfirst(to_search,&fileinfo);                     //��һ�β���
	if(handle==-1)
		return -1;
	                              //��ӡ���ҵ����ļ����ļ���
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
	//tagdistinguish2(head.GetData(),dictionaryroot);              //������һ����ҳ
	while(!_findnext(handle,&fileinfo))                         //�����������ϵ��ļ�
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
		//tagdistinguish2(head.GetData(),dictionaryroot);          //��������ҳ
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
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

