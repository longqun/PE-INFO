// ResouceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PELoad.h"
#include "ResouceDlg.h"
#include "afxdialogex.h"
#include "map"
#include "vector"
#include "info.h"
#include "Log.h"
#include "function.h"

// ResouceDlg 对话框

IMPLEMENT_DYNAMIC(ResouceDlg, CDialogEx)

extern std::map<int, std::vector<ResouceNode>> resouceMap;
extern PVOID fileAddress;

extern std::vector<SectionNode>sectionVector;
ResouceNode *currentNode;

bool isIcon;
extern IMAGE_RESOURCE_DIRECTORY* myoriginal;;

extern std::map<int, RSCRootNode>rootMap;;

static char s_buf[100];
ResouceDlg::ResouceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

ResouceDlg::~ResouceDlg()
{
}

void ResouceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, IDC_PIC, m_pic);
}

CString ResouceDlg::getName(DWORD id)
{
	switch (id)
	{
	case 1:
		return L"光标";
		break;
	case 2:
		return L"位图";
		break;
	case 3:
		return L"图标";
		break;
	case 4:
		return L"菜单";
		break;
	case 5:
		return L"对话框";
		break;
	case 6:
		return L"字符串";
		break;
	case 7:
		return L"字体目录";
		break;
	case 8:
		return L"字体";
		break;
	case 9:
		return L"加速键";
		break;
	case 0xA:
		return L"未格式资源";
		break;
	case 0xB:
		return L"消息表";
		break;
	case 0xC:
		return L"光标组";
		break;
	case 0xE:
		return L"图标组";
		break;
	case 0x10:
		return L"版本信息";
		break;
	default:
		break;
	}
	CString cs;
	cs.Format(L"%d", id);
	return cs;
}


BEGIN_MESSAGE_MAP(ResouceDlg, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &ResouceDlg::OnSelchangedTree)
	ON_BN_CLICKED(IDC_SAVE, &ResouceDlg::OnBnClickedSave)
END_MESSAGE_MAP()


// ResouceDlg 消息处理程序


void ResouceDlg::SetItemText(CString text, DWORD id)
{
	SetDlgItemTextW(id, text);
}

CString ResouceDlg::GetItemTextHex(DWORD hex)
{
	CString cs;
	cs.Format(L"%08X", hex);
	return cs;
}


BOOL ResouceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HTREEITEM hRoot;     // 树的根节点的句柄
	int i = 0;
	
	auto rootName = rootMap.begin();
	

	
	for (auto temp = resouceMap.begin(); temp != resouceMap.end(); temp++)
	{
		if (rootName->second.name != NULL)
		{
			CString cs(rootName->second.name->NameString, rootName->second.name->Length);
			hRoot = m_tree.InsertItem(cs, TVI_ROOT);
		}
		else
			hRoot = m_tree.InsertItem(getName(temp->first), TVI_ROOT);
		rootName++;

		for (int j = 0; j < temp->second.size(); j++)
		{
			HTREEITEM tempRoot = m_tree.InsertItem(GetItemTextHex(temp->second[j].id), hRoot);
			m_tree.SetItemData(tempRoot, (DWORD_PTR)&temp->second[j]);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//not use
void  ResouceDlg::LoadMemImage(void *  pMemData, long  len)
{
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, len);
	void *  pData = GlobalLock(hGlobal);
	memcpy(pData, pMemData, len);
	GlobalUnlock(hGlobal);

	IStream *  pStream = NULL;
	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		CImage image;
		if (SUCCEEDED(image.Load(pStream)))
		{
			m_pic.SetBitmap(image);
		}
		else
		{
			MessageBox(0, 0, 0);
		}
		pStream->Release();
	}
	else
	{
		MessageBox(0, 0, 1);
	}
	GlobalFree(hGlobal);

}


void ResouceDlg::OnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	HTREEITEM hItem = m_tree.GetSelectedItem();
	DWORD_PTR ptr = m_tree.GetItemData(hItem);
	ResouceNode *node = NULL;
	HTREEITEM hParent = m_tree.GetParentItem(hItem);
	if (hParent != NULL)
	{
		CString cs = m_tree.GetItemText(hParent);
		node = (ResouceNode*)ptr;
		currentNode = node;
		//LoadMemImage((void*)((DWORD)fileAddress+node->fileOffset),node->size);
		m_pic.ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
		if (cs == L"图标")
		{
			isIcon = true;
			BITMAPINFO*bmpInfo = (BITMAPINFO*)((DWORD)fileAddress + node->fileOffset);

			HBITMAP bitmap = CreateBitmap(bmpInfo->bmiHeader.biWidth, bmpInfo->bmiHeader.biWidth, bmpInfo->bmiHeader.biPlanes, bmpInfo->bmiHeader.biBitCount, (void*)((DWORD)bmpInfo + sizeof(bmpInfo->bmiHeader)));
			if (bitmap != NULL)
				m_pic.SetBitmap(bitmap);
			else
			{
				sprintf_s(s_buf, "last error %d", GetLastError());
				Log::getInstance()->log_error(0, " ", s_buf);;
			}
		}
		else
		{
			isIcon = false;
		}

	}

	if (node != NULL)
	{
		SetDlgItemTextW(IDC_RVA, GetItemTextHex(node->rva));
		SetDlgItemTextW(IDC_FILEOFFSET, GetItemTextHex(node->fileOffset));
		SetDlgItemTextW(IDC_SIZEFILE, GetItemTextHex(node->size));
	}



}

CString ResouceDlg::Show()
{
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	CString         strFolderPath = TEXT("");

	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("请选择一个文件夹：");
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	sInfo.lpfn = NULL;

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			strFolderPath = szFolderPath;
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}

	return strFolderPath;

}

void ResouceDlg::writeICO(CString path)
{
	BITMAPINFOHEADER *pB = (PBITMAPINFOHEADER)((DWORD)fileAddress + currentNode->fileOffset);
	ICONDIR icon = { 0 };
	icon.idReserved = 0;
	icon.idType = 1;
	icon.idCount = 1;
	icon.idEntries[0].bWidth = pB->biWidth;
	icon.idEntries[0].bHeight = pB->biHeight;
	icon.idEntries[0].bColorCount = 16;
	icon.idEntries[0].bReserved = 0;
	icon.idEntries[0].wPlanes = 0;
	icon.idEntries[0].wBitCount = pB->biBitCount;
	//icon数据大小
	icon.idEntries[0].dwBytesInRes = currentNode->size;
	icon.idEntries[0].dwImageOffset = 22;

	HANDLE handle = CreateNewFile(path.GetBuffer());
	DWORD ret;
	WriteFile(handle, (PVOID)&icon, sizeof(icon), &ret, 0);
	WriteFile(handle, (LPVOID)((DWORD)fileAddress + currentNode->fileOffset), currentNode->size, &ret, 0);
	CloseHandle(handle);
}

void ResouceDlg::writeData(CString path)
{
	DWORD ret;
	HANDLE handle = CreateNewFile(path.GetBuffer());
	WriteFile(handle, (LPVOID)((DWORD)fileAddress + currentNode->fileOffset), currentNode->size, &ret, 0);
	CloseHandle(handle);
}

void ResouceDlg::OnBnClickedSave()
{
	CString cs;
	GetDlgItemTextW(IDC_FILEOFFSET, cs);
	if (cs.IsEmpty())
	{
		MessageBox(L"not null", 0, 0);
		return;
	}
	else
	{
		CString path = Show();
		if (!path.IsEmpty())
		{

			path += "//";
			CString fileName;
			if (isIcon)
			{
				fileName.Format(L"%d.ico", currentNode->id);
				path += +fileName;
				writeICO(path);
			}
			else
			{
				fileName.Format(L"%d.dat", currentNode->id);
				path += +fileName;
				writeData(path);
			}
		}
	}
}
