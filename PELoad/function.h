#pragma once
#include "windows.h"
#include "info.h"
#include "vector"
#define IsPE(buf) buf[0]==0x4d&&buf[1]==0x5a

HANDLE MyOpenFile(TCHAR*path);

HANDLE CreateNewFile(TCHAR*path);

bool isPE(LPBYTE buf);

IMAGE_FILE_HEADER* getIMAGE_FILE_HEADER(LPBYTE buf);

DWORD RVAToOffset(DWORD rva, std::vector<SectionNode>&vec);
DWORD OffsetToRVA(DWORD offset, std::vector<SectionNode>& vec);

IMAGE_NT_HEADERS32 *getIMAGE_NT_HEAD32(LPBYTE buf);
IMAGE_NT_HEADERS64 *getIMAGE_NT_HEAD64(LPBYTE buf);

PIMAGE_IMPORT_DESCRIPTOR getFirstIMAGE_IMPORT_DESCRIPTOR32(LPBYTE buf, std::vector<SectionNode>&vec);
PIMAGE_IMPORT_DESCRIPTOR getFirstIMAGE_IMPORT_DESCRIPTOR64(LPBYTE buf, std::vector<SectionNode>&vec);

IMAGE_SECTION_HEADER * getFirstIMAGE_SECTION_HEADER32(LPBYTE buf);
IMAGE_SECTION_HEADER * getFirstIMAGE_SECTION_HEADER64(LPBYTE buf);

IMAGE_OPTIONAL_HEADER32* getIMAGE_OPTIONAL_HEADER32(LPBYTE buf);
IMAGE_OPTIONAL_HEADER64* getIMAGE_OPTIONAL_HEADER64(LPBYTE buf);

IMAGE_EXPORT_DIRECTORY* getIMAGE_EXPORT_DIRECTORY32(LPBYTE buf, std::vector<SectionNode>&vec);
IMAGE_EXPORT_DIRECTORY* getIMAGE_EXPORT_DIRECTORY64(LPBYTE buf, std::vector<SectionNode>&vec);






