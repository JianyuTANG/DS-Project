#pragma once


// cmydialog �Ի���

class cmydialog : public CDialogEx
{
	DECLARE_DYNAMIC(cmydialog)

public:
	cmydialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~cmydialog();

// �Ի�������
	enum { IDD = IDD_TIP_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
