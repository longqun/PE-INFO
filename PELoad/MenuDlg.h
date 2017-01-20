#pragma once
#include "afxcmn.h"


// MenuDlg 对话框

class MenuDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MenuDlg)

public:
	MenuDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MenuDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
};
