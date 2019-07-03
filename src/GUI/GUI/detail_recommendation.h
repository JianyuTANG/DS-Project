#pragma once


// detail_recommendation 对话框

class detail_recommendation : public CDialogEx
{
	DECLARE_DYNAMIC(detail_recommendation)

public:
	detail_recommendation(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~detail_recommendation();
	detail_recommendation *m_next1;
	detail_recommendation *m_next2;
	detail_recommendation *m_next3;
	detail_recommendation *m_next4;
	detail_recommendation *m_next5;
	int filecode[6];

// 对话框数据
	enum { IDD = IDD_detail_DIALOG1 };
	void display(int urlcode);
	void recommendation(int urlcode);
	void cannotfindmore();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString recommendation1;
	CString recommendation2;
	CString recommendation3;
	CString recommendation4;
	CString recommendation5;
	CString maintext;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
