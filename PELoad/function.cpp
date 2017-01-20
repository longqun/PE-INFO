
#include "function.h"
#include "log.h"

HANDLE MyOpenFile(TCHAR*path)
{
	HANDLE retHandle = CreateFileW(path, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (retHandle == INVALID_HANDLE_VALUE)
	{
		Log*log = Log::getInstance();
		log->log_error(__LINE__, __FILE__, "Open file error");
		return INVALID_HANDLE_VALUE;
	}
	return retHandle;
}

HANDLE CreateNewFile(TCHAR*path)
{
	HANDLE retHandle = CreateFileW(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (retHandle == INVALID_HANDLE_VALUE)
	{
		Log*log = Log::getInstance();
		log->log_error(__LINE__, __FILE__, "Create file error");
		return INVALID_HANDLE_VALUE;
	}
	return retHandle;
}

bool isPE(LPBYTE buf)
{
	if (buf[0] == 0x4d && buf[1] == 0x5a)
	{
		LPBYTE sign = buf + (DWORD)((IMAGE_DOS_HEADER*)buf)->e_lfanew;
		if (sign[0] == 'P'&&sign[1] == 'E')
			return true;
		return false;
	}
	return false;
}

IMAGE_FILE_HEADER* getIMAGE_FILE_HEADER(LPBYTE buf)
{
	LPBYTE inh = (buf + (DWORD)((IMAGE_DOS_HEADER*)buf)->e_lfanew);
	LPBYTE ret = inh + sizeof(DWORD);
	return (IMAGE_FILE_HEADER*)ret;
}

IMAGE_NT_HEADERS32 *getIMAGE_NT_HEAD32(LPBYTE buf)
{
	LPBYTE inh = (buf + (DWORD)((IMAGE_DOS_HEADER*)buf)->e_lfanew);
	return (IMAGE_NT_HEADERS32*)inh;
}

IMAGE_NT_HEADERS64 *getIMAGE_NT_HEAD64(LPBYTE buf)
{
	LPBYTE inh = (buf + (DWORD)((IMAGE_DOS_HEADER*)buf)->e_lfanew);
	return (IMAGE_NT_HEADERS64*)inh;
}


DWORD RVAToOffset(DWORD rva, std::vector<SectionNode>& vec)
{
	for (int i = 0; i < vec.size(); i++)
	{
		if (rva >= vec[i].rva&&rva < (vec[i].size + vec[i].rva))
		{
			return vec[i].fileOffset + rva - vec[i].rva;
		}
	}
	return -1;
}

PIMAGE_IMPORT_DESCRIPTOR getFirstIMAGE_IMPORT_DESCRIPTOR32(LPBYTE buf, std::vector<SectionNode>&vec)
{
	PIMAGE_DATA_DIRECTORY importTable = &getIMAGE_OPTIONAL_HEADER32(buf)->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	if (importTable->VirtualAddress == 0)
		return NULL;
	return (PIMAGE_IMPORT_DESCRIPTOR)(RVAToOffset(importTable->VirtualAddress,vec)+buf);
}

PIMAGE_IMPORT_DESCRIPTOR getFirstIMAGE_IMPORT_DESCRIPTOR64(LPBYTE buf, std::vector<SectionNode>&vec)
{
	PIMAGE_DATA_DIRECTORY importTable = &getIMAGE_OPTIONAL_HEADER64(buf)->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	return (PIMAGE_IMPORT_DESCRIPTOR)(RVAToOffset(importTable->VirtualAddress, vec) + buf);
}

DWORD OffsetToRVA(DWORD offset, vector<SectionNode>& vec)
{
	for (int i = 0; i < vec.size(); i++)
	{
		if (offset >= vec[i].fileOffset&&offset < (vec[i].fileOffset + vec[i].size))
		{
			return vec[i].rva + offset - vec[i].fileOffset;
		}
	}
	return -1;
}

IMAGE_SECTION_HEADER * getFirstIMAGE_SECTION_HEADER32(LPBYTE buf)
{
	//IMAGE_NT_HEADER
	LPBYTE inh = (buf + (DWORD)((IMAGE_DOS_HEADER*)buf)->e_lfanew);
	inh += sizeof(IMAGE_NT_HEADERS32);
	return (PIMAGE_SECTION_HEADER)inh;
}

IMAGE_SECTION_HEADER * getFirstIMAGE_SECTION_HEADER64(LPBYTE buf)
{
	//IMAGE_NT_HEADER
	LPBYTE inh = (buf + (DWORD)((IMAGE_DOS_HEADER*)buf)->e_lfanew);
	inh += sizeof(IMAGE_NT_HEADERS64);
	return (PIMAGE_SECTION_HEADER)inh;
}

IMAGE_OPTIONAL_HEADER32* getIMAGE_OPTIONAL_HEADER32(LPBYTE buf)
{
	LPBYTE inh = (buf + (DWORD)((IMAGE_DOS_HEADER*)buf)->e_lfanew);
	LPBYTE ret = inh + sizeof(DWORD);
	ret += sizeof(IMAGE_FILE_HEADER);
	return (IMAGE_OPTIONAL_HEADER32*)ret;
}

IMAGE_OPTIONAL_HEADER64* getIMAGE_OPTIONAL_HEADER64(LPBYTE buf)
{
	LPBYTE inh = (buf + (DWORD)((IMAGE_DOS_HEADER*)buf)->e_lfanew);
	LPBYTE ret = inh + sizeof(DWORD);
	ret += sizeof(IMAGE_FILE_HEADER);
	return (IMAGE_OPTIONAL_HEADER64*)ret;
}

IMAGE_EXPORT_DIRECTORY * getIMAGE_EXPORT_DIRECTORY32(LPBYTE buf, std::vector<SectionNode>&vec)
{
	PIMAGE_DATA_DIRECTORY exportTable = &getIMAGE_OPTIONAL_HEADER32(buf)->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	if (exportTable->Size == NULL)
		return NULL;
	return (PIMAGE_EXPORT_DIRECTORY)(RVAToOffset(exportTable->VirtualAddress, vec) + buf);
}

IMAGE_EXPORT_DIRECTORY * getIMAGE_EXPORT_DIRECTORY64(LPBYTE buf, std::vector<SectionNode>&vec)
{
	PIMAGE_DATA_DIRECTORY exportTable = &getIMAGE_OPTIONAL_HEADER64(buf)->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	if (exportTable->Size == 0)
		return NULL;
	return (PIMAGE_EXPORT_DIRECTORY)(RVAToOffset(exportTable->VirtualAddress, vec) + buf);
}
