// MenuDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PELoad.h"
#include "MenuDlg.h"
#include "afxdialogex.h"
#include "vector"

// MenuDlg 对话框

IMPLEMENT_DYNAMIC(MenuDlg, CDialogEx)

std::vector<WCHAR*>nameVec;
extern std::vector<IMAGE_DATA_DIRECTORY>directoryVec;;

static void  initNameVec()
{
	if (nameVec.empty())
	{

		nameVec.push_back(L"输出表");
		nameVec.push_back(L"导入表");
		nameVec.push_back(L"资源");
		nameVec.push_back(L"例外");
		nameVec.push_back(L"安全");
		nameVec.push_back(L"重定位");
		nameVec.push_back(L"调试");
		nameVec.push_back(L"版权");
		nameVec.push_back(L"全局区域");
		nameVec.push_back(L"TLS表");
		nameVec.push_back(L"载入配置");
		nameVec.push_back(L"装订输入");
		nameVec.push_back(L"IAT");
		nameVec.push_back(L"延迟输入");
		nameVec.push_back(L"COM");
		nameVec.push_back(L"保留");
	}
}

MenuDlg::MenuDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{
	initNameVec();
}

MenuDlg::~MenuDlg()
{
}

void MenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(MenuDlg, CDialogEx)
END_MESSAGE_MAP()


// MenuDlg 消息处理程序


BOOL MenuDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;

	// 获取编程语言列表视图控件的位置和大小   
	m_list.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加三列   
	m_list.InsertColumn(0, _T("目录名"), LVCFMT_CENTER, rect.Width() / 3, 0);
	m_list.InsertColumn(1, _T("RVA"), LVCFMT_CENTER, rect.Width() / 3, 1);
	m_list.InsertColumn(2, _T("长度"), LVCFMT_CENTER, rect.Width() / 3, 2);
	initNameVec();
	CString cs;
	for (int i = 0; i < nameVec.size(); i++)
	{
		m_list.InsertItem(i, nameVec[i]);
		cs.Format(L"%08X", directoryVec[i].VirtualAddress);
		m_list.SetItemText(i, 1, cs);
		cs.Format(L"%08X", directoryVec[i].Size);
		m_list.SetItemText(i, 2, cs);
	}



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
