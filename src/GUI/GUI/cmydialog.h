#pragma once


// cmydialog 对话框

class cmydialog : public CDialogEx
{
	DECLARE_DYNAMIC(cmydialog)

public:
	cmydialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~cmydialog();

// 对话框数据
	enum { IDD = IDD_TIP_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
