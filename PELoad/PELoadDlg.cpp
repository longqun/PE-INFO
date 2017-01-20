
// PELoadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PELoad.h"
#include "PELoadDlg.h"
#include "afxdialogex.h"
#include "function.h"
#include "log.h"
#include "info.h"
#include "vector"
#include "ImportDlg.h"
#include "ExportDlg.h"
#include "SectionDlg.h"
#include "MenuDlg.h"
#include "map"
#include "ResouceDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif




HANDLE fileHandle;
HANDLE fileMapping;
PVOID fileAddress;

bool isInit;

IMAGE_RESOURCE_DIR_STRING_U *rootName;

WORD numbersOfSections;

vector<SectionNode>sectionVector;

vector<IMAGE_IMPORT_DESCRIPTOR_NAME>importVec;

vector<vector<APINode>>APIVec;

vector<ExportAPINode>ExportAPIVec;

vector<IMAGE_SECTION_HEADER>SectionDetail;

IMAGE_EXPORT_DIRECTORY_NAME iedn;

vector<IMAGE_DATA_DIRECTORY>directoryVec;

map<int, vector<ResouceNode>> resouceMap;
map<int, RSCRootNode>rootMap;

IMAGE_RESOURCE_DIRECTORY* myoriginal;
bool is64;

void release()
{
	if (fileAddress)
		UnmapViewOfFile(fileAddress);
	if (fileMapping)
		CloseHandle(fileMapping);
	if (fileHandle)
		CloseHandle(fileHandle);
	is64 = false;
	numbersOfSections = 0;
	sectionVector.swap(std::vector<SectionNode>());
	importVec.swap(vector<IMAGE_IMPORT_DESCRIPTOR_NAME>());
	APIVec.swap(vector<vector<APINode>>());
	ExportAPIVec.swap(vector<ExportAPINode>());
	SectionDetail.swap(vector<IMAGE_SECTION_HEADER>());
	directoryVec.swap(vector<IMAGE_DATA_DIRECTORY>());
	resouceMap.swap(map<int, vector<ResouceNode>>());
	ZeroMemory(&iedn, sizeof(iedn));
}

/*
	初始化文件
*/
void CPELoadDlg::InitFile(TCHAR*path)
{
	fileHandle = MyOpenFile(path);
	Log *log = Log::getInstance();
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		log->log_error(__LINE__, __FILE__, 0);
		return;
	}
	DWORD highPart;
	fileMapping = CreateFileMapping(fileHandle, 0, PAGE_READWRITE, 0, GetFileSize(fileHandle, &highPart), NULL);
	if (fileMapping == INVALID_HANDLE_VALUE)
	{
		log->log_error(__LINE__, __FILE__, 0);
		CloseHandle(fileHandle);
		return;
	}
	fileAddress = MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, GetFileSize(fileHandle, &highPart));
	if (fileAddress == NULL)
	{
		CloseHandle(fileHandle);
		CloseHandle(fileMapping);
		log->log_error(__LINE__, __FILE__, 0);
		return;
	}
	isInit = true;
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPELoadDlg 对话框



CPELoadDlg::CPELoadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PELOAD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPELoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPELoadDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_COMMAND(ID_IMPORT, &CPELoadDlg::OnImport)
	ON_COMMAND(ID_EXPORT, &CPELoadDlg::OnExport)
	ON_COMMAND(ID_OPNFILE, &CPELoadDlg::OnOpnfileClick)
	ON_COMMAND(ID_SECTION, &CPELoadDlg::OnSectionDlgClick)
	ON_COMMAND(ID_MENU, &CPELoadDlg::OnMenuDlgClick)
	ON_COMMAND(ID_RESOUCE, &CPELoadDlg::OnResouceDlgClick)
END_MESSAGE_MAP()


// CPELoadDlg 消息处理程序



BOOL CPELoadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}



	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPELoadDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPELoadDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPELoadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPELoadDlg::ImportDlgClick()
{

	IMAGE_SECTION_HEADER *ish;
	if (is64)
	{
		IMAGE_OPTIONAL_HEADER64 *ioh64 = getIMAGE_OPTIONAL_HEADER64((LPBYTE)fileAddress);
		ish = getFirstIMAGE_SECTION_HEADER64((LPBYTE)fileAddress);
		PIMAGE_IMPORT_DESCRIPTOR iid = getFirstIMAGE_IMPORT_DESCRIPTOR64((LPBYTE)fileAddress, sectionVector);
		while (iid != NULL&&iid->Name != NULL&&iid->FirstThunk != NULL)
		{
			char *name = (char*)(RVAToOffset(iid->Name, sectionVector) + (DWORD)fileAddress);
			IMAGE_IMPORT_DESCRIPTOR_NAME iidn;
			iidn.name = name;
			memcpy(&iidn.iid, iid, sizeof(IMAGE_IMPORT_DESCRIPTOR));
			importVec.push_back(iidn);

			IMAGE_THUNK_DATA64 *itd = (IMAGE_THUNK_DATA64*)(RVAToOffset(iid->FirstThunk, sectionVector) + (DWORD)fileAddress);;
			vector<APINode>vec;
			while (itd->u1.AddressOfData != NULL)
			{
				if (itd->u1.AddressOfData > 0x80000000)
				{
					APINode node;
					node.hint = itd->u1.AddressOfData;
					node.name = 0;
					vec.push_back(node);
				}
				else
				{
					IMAGE_IMPORT_BY_NAME* iiby = PIMAGE_IMPORT_BY_NAME(RVAToOffset(itd->u1.AddressOfData, sectionVector) + (DWORD)fileAddress);
					APINode node;
					node.hint = iiby->Hint;
					node.name = (char*)&(iiby->Name);
					vec.push_back(node);
				}
				itd++;
			}
			APIVec.push_back(vec);
			iid++;
		}
	}
	else
	{
		//32位处理
		ish = getFirstIMAGE_SECTION_HEADER32((LPBYTE)fileAddress);
		PIMAGE_IMPORT_DESCRIPTOR iid = getFirstIMAGE_IMPORT_DESCRIPTOR32((LPBYTE)fileAddress, sectionVector);
		while (iid != NULL&&iid->Name != 0)
		{
			char *name = (char*)(RVAToOffset(iid->Name, sectionVector) + (DWORD)fileAddress);
			IMAGE_IMPORT_DESCRIPTOR_NAME iidn;
			iidn.name = name;
			memcpy(&iidn.iid, iid, sizeof(IMAGE_IMPORT_DESCRIPTOR));
			importVec.push_back(iidn);

			IMAGE_THUNK_DATA32 *itd = (IMAGE_THUNK_DATA32*)(RVAToOffset(iid->FirstThunk, sectionVector) + (DWORD)fileAddress);;
			vector<APINode>vec;
			while (itd->u1.AddressOfData != NULL&&iid->FirstThunk != NULL)
			{
				if (itd->u1.AddressOfData > 0x80000000)
				{
					APINode node;
					node.hint = itd->u1.AddressOfData;
					node.name = 0;
					vec.push_back(node);
				}
				else
				{
					IMAGE_IMPORT_BY_NAME* iiby = PIMAGE_IMPORT_BY_NAME(RVAToOffset(itd->u1.AddressOfData, sectionVector) + (DWORD)fileAddress);
					APINode node;
					node.hint = iiby->Hint;
					node.name = (char*)&(iiby->Name);
					vec.push_back(node);
				}
				itd++;
			}
			APIVec.push_back(vec);
			iid++;
		}
	}
}

void CPELoadDlg::SetItemText(CString text, DWORD id)
{
	SetDlgItemTextW(id, text);
}

void CPELoadDlg::SetItemTextHex(DWORD hex, DWORD id)
{
	CString cs;
	cs.Format(L"%08X", hex);
	SetDlgItemTextW(id, cs);
}


void CPELoadDlg::ExportDlgClick()
{
	IMAGE_EXPORT_DIRECTORY*ied = NULL;

	if (is64)
	{
		ied = getIMAGE_EXPORT_DIRECTORY64((PBYTE)fileAddress, sectionVector);
	}
	else
	{
		ied = getIMAGE_EXPORT_DIRECTORY32((PBYTE)fileAddress, sectionVector);
	}
	if (ied == NULL)
		return;
	memcpy(&iedn.ied, ied, sizeof(IMAGE_EXPORT_DIRECTORY));
	char *name = (char*)(RVAToOffset(ied->Name, sectionVector) + (DWORD)fileAddress);
	iedn.name = name;

	DWORD numOfFunctions = ied->NumberOfFunctions;
	DWORD numOfNames = ied->NumberOfNames;
	DWORD*addressOfFunctions = (DWORD*)(RVAToOffset(ied->AddressOfFunctions, sectionVector) + (DWORD)fileAddress);
	DWORD*addressOfNames = (DWORD*)(RVAToOffset(ied->AddressOfNames, sectionVector) + (DWORD)fileAddress);
	WORD*addressOfNameOrdinals = (WORD*)(RVAToOffset(ied->AddressOfNameOrdinals, sectionVector) + (DWORD)fileAddress);
	bool find = false;
	int currentNamesize = 0;
	for (int i = 0; i < ied->NumberOfFunctions; i++)
	{
		if (currentNamesize < ied->NumberOfNames)
		{
			for (int j = 0; j < ied->NumberOfNames; j++)
			{
				if (i == addressOfNameOrdinals[j])
				{
					ExportAPINode node;
					node.name = (char*)(RVAToOffset(addressOfNames[j], sectionVector) + (DWORD)fileAddress);
					node.Ordinal = i;
					node.rva = addressOfFunctions[i];
					ExportAPIVec.push_back(node);
					currentNamesize++;
					find = true;
					break;
				}
			}
		}
		else
		{
			ExportAPINode node;
			node.name = 0;
			node.Ordinal = i;
			node.rva = addressOfFunctions[i];
			ExportAPIVec.push_back(node);
		}

		if (!find)
		{
			ExportAPINode node;
			node.name = 0;
			node.Ordinal = i;
			node.rva = addressOfFunctions[i];
			ExportAPIVec.push_back(node);
		}
		find = false;
	}
}

void  CPELoadDlg::ShowBasicInfo(PVOID ioh)
{
	IMAGE_FILE_HEADER *ifh = getIMAGE_FILE_HEADER((LPBYTE)fileAddress);
	SetItemTextHex(ifh->TimeDateStamp, IDC_FILETIME);
	SetItemTextHex(ifh->Characteristics, IDC_CHARACTERISTICS);
	SetItemTextHex(ifh->SizeOfOptionalHeader, IDC_OPTIONALHEADERLENGTH);
	SetItemTextHex(ifh->NumberOfSections, IDC_SECTIONCOUNT);
	IMAGE_OPTIONAL_HEADER32 *ioh32 = (IMAGE_OPTIONAL_HEADER32 *)ioh;
	IMAGE_OPTIONAL_HEADER64 *ioh64 = (IMAGE_OPTIONAL_HEADER64 *)ioh;
	if (is64)
	{
		DWORD high;
		SetItemTextHex(ioh64->Magic, IDC_MAGIC);
		SetItemTextHex(ioh64->MajorLinkerVersion, IDC_LINKERVERSION);
		SetItemTextHex(ioh64->AddressOfEntryPoint, IDC_ENTRYPOINT);
		SetItemTextHex(ioh64->ImageBase, IDC_BASEADDRESS);
		SetItemTextHex(ioh64->SectionAlignment, IDC_SECTIONALIGMENT);
		SetItemTextHex(ioh64->FileAlignment, IDC_FILEALIGMENT);
		SetItemTextHex(ioh64->Subsystem, IDC_SUBSYSTEM);
		SetItemTextHex(ioh64->BaseOfCode, IDC_CODESTART);
		SetItemTextHex(ioh64->SizeOfImage, IDC_SIZEOFIMAGE);
		SetItemTextHex(GetFileSize(fileHandle, &high), IDC_FILESIZE);
		SetItemTextHex(ioh64->CheckSum, IDC_CHECKSUM);
	}
	else
	{
		DWORD high;
		SetItemTextHex(ioh32->Magic, IDC_MAGIC);
		SetItemTextHex(ioh32->MajorLinkerVersion, IDC_LINKERVERSION);
		SetItemTextHex(ioh32->AddressOfEntryPoint, IDC_ENTRYPOINT);
		SetItemTextHex(ioh32->ImageBase, IDC_BASEADDRESS);
		SetItemTextHex(ioh32->SectionAlignment, IDC_SECTIONALIGMENT);
		SetItemTextHex(ioh32->FileAlignment, IDC_FILEALIGMENT);
		SetItemTextHex(ioh32->Subsystem, IDC_SUBSYSTEM);
		SetItemTextHex(ioh32->BaseOfCode, IDC_CODESTART);
		SetItemTextHex(ioh32->SizeOfImage, IDC_SIZEOFIMAGE);
		SetItemTextHex(GetFileSize(fileHandle, &high), IDC_FILESIZE);
		SetItemTextHex(ioh32->CheckSum, IDC_CHECKSUM);
	}

}

int id;


void CPELoadDlg::deepFind(IMAGE_RESOURCE_DIRECTORY *ird, int depth, IMAGE_RESOURCE_DIRECTORY *orignal, vector<ResouceNode>&rscVec)
{
	if (depth == 3)
	{

		PIMAGE_RESOURCE_DIRECTORY_ENTRY irde = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(++ird);
		IMAGE_RESOURCE_DATA_ENTRY *irdatae = (IMAGE_RESOURCE_DATA_ENTRY *)((DWORD)orignal + irde->OffsetToData);
		DWORD offset = RVAToOffset(irdatae->OffsetToData, sectionVector);
		ResouceNode node(irdatae->OffsetToData, irdatae->Size, offset, id);
		rscVec.push_back(node);
		return;
	}
	int size = ird->NumberOfIdEntries + ird->NumberOfNamedEntries;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY irde = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(++ird);
	for (int i = 0; i < size; i++)
	{
		if (!irde[i].DataIsDirectory)
		{
			PIMAGE_RESOURCE_DIRECTORY_ENTRY irde = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(++ird);
			IMAGE_RESOURCE_DATA_ENTRY *irdatae = (IMAGE_RESOURCE_DATA_ENTRY *)((DWORD)orignal + irde->OffsetToData);
			DWORD offset = RVAToOffset(irdatae->OffsetToData, sectionVector);
			ResouceNode node(irdatae->OffsetToData, irdatae->Size, offset, id);
			rscVec.push_back(node);
		}
		else
		{
			id = irde[i].Id;
			deepFind((PIMAGE_RESOURCE_DIRECTORY)(irde[i].OffsetToDirectory + (DWORD)orignal), depth + 1, orignal, rscVec);
		}
	}
}

void  CPELoadDlg::getResource()
{
	IMAGE_RESOURCE_DIRECTORY *ird = NULL;
	if (is64)
	{
		IMAGE_OPTIONAL_HEADER64 *ioh = getIMAGE_OPTIONAL_HEADER64((LPBYTE)fileAddress);
		DWORD rva = ioh->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
		if (ioh->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size == 0|| ioh->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress==0)
			return;
		ird = (IMAGE_RESOURCE_DIRECTORY *)(RVAToOffset(rva, sectionVector) + (DWORD)fileAddress);
	}
	else
	{
		IMAGE_OPTIONAL_HEADER32 *ioh = getIMAGE_OPTIONAL_HEADER32((LPBYTE)fileAddress);
		if (ioh->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size == 0 || ioh->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress==0)
			return;
		DWORD rva = ioh->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
		ird = (IMAGE_RESOURCE_DIRECTORY *)(RVAToOffset(rva, sectionVector) + (DWORD)fileAddress);
	}
	IMAGE_RESOURCE_DIRECTORY*orignal = ird;
	int size = ird->NumberOfIdEntries + ird->NumberOfNamedEntries;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY irde = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)++ird;
	//myoriginal = orignal;
	for (int i = 0; i < size; i++)
	{
		IMAGE_RESOURCE_DIRECTORY *temp = (IMAGE_RESOURCE_DIRECTORY*)(irde[i].OffsetToDirectory + (DWORD)orignal);
		deepFind(temp, 2, orignal, resouceMap[irde[i].Name]);
		if (irde[i].NameIsString)
		{
			RSCRootNode _node;
			_node.name = (IMAGE_RESOURCE_DIR_STRING_U*)(irde[i].NameOffset + (DWORD)orignal);
			rootMap.insert(make_pair(irde[i].Name, _node));
		}
		else
		{
			RSCRootNode _node;
			rootMap.insert(make_pair(irde[i].Name, _node));
		}
	}


}

void CPELoadDlg::initParam()
{

	IMAGE_OPTIONAL_HEADER* ioh = getIMAGE_OPTIONAL_HEADER32((LPBYTE)fileAddress);

	if (ioh->Magic == 0x20b)
	{
		is64 = true;
	}
	else
	{
		is64 = false;
	}
	ShowBasicInfo(ioh);
	IMAGE_SECTION_HEADER *ish;
	numbersOfSections = getIMAGE_FILE_HEADER((LPBYTE)fileAddress)->NumberOfSections;
	//64位处理
	if (is64)
	{
		IMAGE_OPTIONAL_HEADER64 *ioh64 = (PIMAGE_OPTIONAL_HEADER64)ioh;
		ish = getFirstIMAGE_SECTION_HEADER64((LPBYTE)fileAddress);
	}
	else
	{
		//32位处理
		IMAGE_OPTIONAL_HEADER32 *ioh32 = ioh;
		ish = getFirstIMAGE_SECTION_HEADER32((LPBYTE)fileAddress);
	}

	for (int i = 0; i < numbersOfSections; i++)
	{
		SectionNode node(ish[i].VirtualAddress, ish[i].PointerToRawData, ish[i].SizeOfRawData);
		sectionVector.push_back(node);
		SectionDetail.push_back(ish[i]);
	}

}




void CPELoadDlg::OnImport()
{
	if (!isInit)
	{
		MessageBoxW(0, L"Not init!", 0);
		return;
	}
	if (importVec.empty() && APIVec.empty())
		ImportDlgClick();
	ImportDlg dlg;
	dlg.DoModal();
}


void CPELoadDlg::OnExport()
{
	if (!isInit)
	{
		MessageBoxW(0, L"Not init!", 0);
		return;
	}
	if (ExportAPIVec.empty())
		ExportDlgClick();
	ExportDlg dlg;
	dlg.DoModal();
}


void CPELoadDlg::OnOpnfileClick()
{

	CString strFile = _T("");
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("PE |*.exe;*.dll;*.com;*.sys|All Files (*.*)|.*||"), NULL);

	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}

	if (!strFile.IsEmpty())
	{
		if (isInit)
		{
			release();
		}
		InitFile(strFile.GetBuffer());
		if (isInit)
			initParam();
		if (!isInit)
		{
			MessageBoxA(0, "Init fail", 0, 0);
		}
	}

}


void CPELoadDlg::OnSectionDlgClick()
{
	if (!isInit)
	{
		MessageBoxW(0, L"Not init!", 0);
		return;
	}
	SectionDlg dlg;
	dlg.DoModal();
}

void CPELoadDlg::MenuClick()
{
	IMAGE_DATA_DIRECTORY *idd;
	if (is64)
	{
		IMAGE_OPTIONAL_HEADER64 *ioh = getIMAGE_OPTIONAL_HEADER64((LPBYTE)fileAddress);
		idd = ioh->DataDirectory;
	}
	else
	{
		IMAGE_OPTIONAL_HEADER32 *ioh = getIMAGE_OPTIONAL_HEADER32((LPBYTE)fileAddress);
		idd = ioh->DataDirectory;
	}

	for (int i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i++)
	{
		directoryVec.push_back(idd[i]);
	}
}

void CPELoadDlg::OnMenuDlgClick()
{
	if (!isInit)
	{
		MessageBox(L"not init!", 0, 0);
		return;
	}
	if (directoryVec.empty())
		MenuClick();
	MenuDlg dlg;
	dlg.DoModal();
}


void CPELoadDlg::OnResouceDlgClick()
{
	if (!isInit)
	{
		MessageBox(L"not init!", 0, 0);
		return;
	}

	if (resouceMap.empty())
		getResource();
	ResouceDlg dlg;
	dlg.DoModal();
}
