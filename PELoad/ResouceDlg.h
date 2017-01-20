#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// ResouceDlg 对话框

class ResouceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ResouceDlg)

public:
	ResouceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ResouceDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	void SetItemText(CString text, DWORD id);
	CString ResouceDlg::GetItemTextHex(DWORD hex);
	CString getName(DWORD id);
	void  LoadMemImage(void *  pMemData, long  len);
	void writeData(CString path);
	void writeICO(CString path);
	CString Show();
public:
	CTreeCtrl m_tree;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_pic;
	afx_msg void OnBnClickedSave();
};
