#pragma once
#include "detail_recommendation.h"

// search_result �Ի���

class search_result : public CDialogEx
{
	DECLARE_DYNAMIC(search_result)

public:
	search_result(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~search_result();
	void get_result_list(linknode *temp);
	detail_recommendation *detail_1;
	//CharStringLink* my_result;
// �Ի�������
	enum { IDD = IDD_search_result_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int filecode[6];
	CString result1;
	CString reslut2;
	CString result3;
	CString result4;
	CString result5;
	afx_msg void OnBnClickeddetail1();
	afx_msg void OnBnClickeddetail2();
	afx_msg void OnBnClickeddetail3();
	afx_msg void OnBnClickeddetail4();
	afx_msg void OnBnClickeddetail5();
};
