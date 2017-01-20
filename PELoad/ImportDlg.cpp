// ImportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PELoad.h"
#include "ImportDlg.h"
#include "afxdialogex.h"
#include "vector"
#include "info.h"
#include "function.h"
extern std::vector<SectionNode>sectionVector;
extern bool is64;
extern WORD numbersOfSections;
extern PVOID fileAddress;

extern std::vector<IMAGE_IMPORT_DESCRIPTOR_NAME>importVec;
extern std::vector<std::vector<APINode>>APIVec;

// ImportDlg 对话框

IMPLEMENT_DYNAMIC(ImportDlg, CDialogEx)

ImportDlg::ImportDlg( CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IMPORTDLG, pParent)
{

}


ImportDlg::~ImportDlg()
{
}

void ImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LIST2, m_listdetail);
}


BEGIN_MESSAGE_MAP(ImportDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &ImportDlg::OnNMClickListShowDetail)
END_MESSAGE_MAP()


// ImportDlg 消息处理程序


BOOL ImportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;


	m_list.GetClientRect(&rect);


	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	m_list.InsertColumn(0, _T("DLLNAME"), LVCFMT_CENTER, rect.Width() / 6, 0);
	m_list.InsertColumn(1, _T("OriginalFirstThunk"), LVCFMT_CENTER, rect.Width() / 6, 1);
	m_list.InsertColumn(2, _T("TimeDateStamp"), LVCFMT_CENTER, rect.Width() / 6, 2);
	m_list.InsertColumn(3, _T("ForwardChain"), LVCFMT_CENTER, rect.Width() / 6, 3);
	m_list.InsertColumn(4, _T("NameAddress"), LVCFMT_CENTER, rect.Width() / 6, 4);
	m_list.InsertColumn(5, _T("FirstThunk"), LVCFMT_CENTER, rect.Width() / 6, 5);

	m_listdetail.GetClientRect(&rect);
	m_listdetail.SetExtendedStyle(m_listdetail.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	m_listdetail.InsertColumn(0, _T(""), LVCFMT_CENTER, 25, 0);
	m_listdetail.InsertColumn(1, _T("Hint"), LVCFMT_CENTER, rect.Width() / 2 - 40, 1);
	m_listdetail.InsertColumn(2, _T("ApiName"), LVCFMT_CENTER, rect.Width() / 2 - 40, 2);
	CString cs;
	for (int i = 0; i < importVec.size(); i++)
	{
		m_list.InsertItem(i, CString(importVec[i].name));
		cs.Format(L"%08X", importVec[i].iid.OriginalFirstThunk);
		m_list.SetItemText(i,1,cs);
		cs.ReleaseBuffer();
		cs.Format(L"%08X", importVec[i].iid.TimeDateStamp);
		m_list.SetItemText(i, 2, cs);
		cs.ReleaseBuffer();
		cs.Format(L"%08X", importVec[i].iid.ForwarderChain);
		m_list.SetItemText(i, 3, cs);
		cs.ReleaseBuffer();
		cs.Format(L"%08X", importVec[i].iid.Name);
		m_list.SetItemText(i, 4, cs);
		cs.ReleaseBuffer();
		cs.Format(L"%08X", importVec[i].iid.FirstThunk);
		m_list.SetItemText(i, 5, cs);
	}

	return TRUE;
}


void ImportDlg::OnNMClickListShowDetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	m_listdetail.DeleteAllItems();
	if (-1 != pNMListView->iItem)
	{

		if (pNMListView->iItem<APIVec.size())
		{
			CString cs;
			for (int i = 0; i < APIVec[pNMListView->iItem].size(); i++)
			{
				cs.Format(L"%d", i);
				m_listdetail.InsertItem(i,cs);
				cs.ReleaseBuffer();
				cs.Format(L"%08X", APIVec[pNMListView->iItem][i].hint);
				m_listdetail.SetItemText(i,1,cs);
				cs.ReleaseBuffer();
				m_listdetail.SetItemText(i, 2, CString(APIVec[pNMListView->iItem][i].name));
			}
		}
	}
	*pResult = 0;
}
