
// GUIDlg.h : ͷ�ļ�
//

#pragma once
#include "cmydialog.h"
#include "search_result.h"



// CGUIDlg �Ի���
class CGUIDlg : public CDialogEx
{
// ����
public:
	CGUIDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GUI_DIALOG };
	cmydialog *mdlg;
	search_result *myresult;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_editsummond;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedaddButton1();
	afx_msg void OnStnClickedsummondStatic();
};
