// ExportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PELoad.h"
#include "ExportDlg.h"
#include "afxdialogex.h"
#include "info.h"
#include "vector"
extern std::vector<ExportAPINode>ExportAPIVec;

extern IMAGE_EXPORT_DIRECTORY_NAME iedn;

// ExportDlg 对话框

IMPLEMENT_DYNAMIC(ExportDlg, CDialogEx)

ExportDlg::ExportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

ExportDlg::~ExportDlg()
{
}

void ExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(ExportDlg, CDialogEx)
END_MESSAGE_MAP()


// ExportDlg 消息处理程序


BOOL ExportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;

	// 获取编程语言列表视图控件的位置和大小   
	m_list.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加三列   
	
	m_list.InsertColumn(0, _T("Ordinal"), LVCFMT_CENTER, rect.Width() / 3 - 20, 0);
	m_list.InsertColumn(1, _T("RVA"), LVCFMT_CENTER, rect.Width() / 3 - 20, 1);
	m_list.InsertColumn(2, _T("NAME"), LVCFMT_CENTER, rect.Width() / 3 - 20, 2);
	CString cs;
	for (int i = 0; i < ExportAPIVec.size(); i++)
	{
		cs.ReleaseBuffer();
		cs.Format(L"%04X", ExportAPIVec[i].Ordinal);
		m_list.InsertItem(i, cs);
		cs.ReleaseBuffer();
		cs.Format(L"%8X", ExportAPIVec[i].rva);
		m_list.SetItemText(i, 1,cs);
		if (ExportAPIVec[i].name != NULL)
		{
			cs.ReleaseBuffer();
			m_list.SetItemText(i, 2, CString(ExportAPIVec[i].name));
		}
		else
		{
			cs.ReleaseBuffer();
			cs.Format(L"%s", "--");
			m_list.SetItemText(i, 2, cs);
		}
	}


	SetDlgItemTextW(IDC_FILENAME,CString(iedn.name));

	cs.Format(L"%08X",iedn.ied.NumberOfFunctions);
	SetDlgItemTextW(IDC_FUNCTIONNUM,cs);
	cs.Format(L"%08X", iedn.ied.NumberOfNames);
	SetDlgItemTextW(IDC_FUNCTIONNAMENUM,cs);
	cs.Format(L"%08X",iedn.ied.AddressOfFunctions);
	SetDlgItemTextW(IDC_FUNCTIONADDR,cs);
	cs.Format(L"%08X",iedn.ied.AddressOfNames);
	SetDlgItemTextW(IDC_FUNCTIONNAMEADDR,cs);
	cs.Format(L"%08X",iedn.ied.AddressOfNameOrdinals);
	SetDlgItemTextW(IDC_FUNCTIONNUMADDR,cs);


	return TRUE;
}
