
// GUIDlg.h : 头文件
//

#pragma once
#include "cmydialog.h"
#include "search_result.h"



// CGUIDlg 对话框
class CGUIDlg : public CDialogEx
{
// 构造
public:
	CGUIDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GUI_DIALOG };
	cmydialog *mdlg;
	search_result *myresult;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
