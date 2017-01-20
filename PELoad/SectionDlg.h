#pragma once
#include "afxcmn.h"


// SectionDlg 对话框

class SectionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SectionDlg)

public:
	SectionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SectionDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
};
