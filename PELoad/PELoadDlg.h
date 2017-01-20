
// PELoadDlg.h : 头文件
//

#pragma once
#include "vector"
#include "info.h"
// CPELoadDlg 对话框
class CPELoadDlg : public CDialogEx
{
// 构造
public:
	CPELoadDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PELOAD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	private:
		void InitFile(TCHAR*path);
		void initParam();
		void SetItemText(CString text, DWORD id);
		void SetItemTextHex(DWORD hex, DWORD id);
		void ShowBasicInfo( PVOID ioh);
		void MenuClick();
		void getResource();
		void deepFind(IMAGE_RESOURCE_DIRECTORY *ird, int depth, IMAGE_RESOURCE_DIRECTORY *orignal, std::vector<ResouceNode>&rscVec);
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
	

private:
	void ExportDlgClick();
	void ImportDlgClick();
public:
	afx_msg void OnImport();
	afx_msg void OnExport();
	afx_msg void OnOpnfileClick();
	afx_msg void OnSectionDlgClick();
	afx_msg void OnMenuDlgClick();
	afx_msg void OnResouceDlgClick();
};
