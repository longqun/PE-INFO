#pragma once
#include "afxcmn.h"


// ImportDlg 对话框

class ImportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ImportDlg)

public:
	ImportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ImportDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	CListCtrl m_listdetail;
	IMAGE_SECTION_HEADER*ish_;
	afx_msg void OnNMClickListShowDetail(NMHDR *pNMHDR, LRESULT *pResult);
};
