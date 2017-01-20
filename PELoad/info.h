#pragma once
#include "windows.h"
typedef struct _SectionNode
{
	DWORD rva;
	DWORD fileOffset;
	DWORD size;
	_SectionNode(DWORD _rva, DWORD _fileOffset, DWORD _size) :rva(_rva), fileOffset(_fileOffset), size(_size) {}
}SectionNode, *PSeciontNode;

typedef struct _IMAGE_IMPORT_DESCRIPTOR_NAME {
	IMAGE_IMPORT_DESCRIPTOR iid;
	char *name;
}IMAGE_IMPORT_DESCRIPTOR_NAME, *PIMAGE_IMPORT_DESCRIPTOR_NAME;

typedef struct _APINode {
	WORD hint;
	char *name;
}APINode, *PAPINode;

typedef struct _ExportAPINode
{
	DWORD Ordinal;
	DWORD rva;
	char *name;
}ExportAPINode, *PExportAPINode;

typedef struct _IMAGE_EXPORT_DIRECTORY_NAME {
	IMAGE_EXPORT_DIRECTORY ied;
	char *name;
}IMAGE_EXPORT_DIRECTORY_NAME, *PIMAGE_EXPORT_DIRECTORY_NAME;


typedef struct _ResouceNode {
	DWORD rva;
	DWORD size;
	DWORD fileOffset;
	DWORD id;
	IMAGE_RESOURCE_DIR_STRING_U*name;
	_ResouceNode(DWORD prva, DWORD psize, DWORD pfileOffset, DWORD pid) :rva(prva), size(psize), fileOffset(pfileOffset), id(pid), name(NULL)
	{

	}
}ResouceNode, *PResouceNode;

typedef struct _RSCRootNode
{
	IMAGE_RESOURCE_DIR_STRING_U *name;
	DWORD id;
	_RSCRootNode() :name(NULL), id(0)
	{}
}RSCRootNode, *PRSCRootNode;


//按照1个字节对齐
#pragma pack (1)

//icon头数据结构
typedef struct
{
	BYTE bWidth; // Width, in pixels, of the image
	BYTE bHeight; // Height, in pixels, of the image
	BYTE bColorCount; // Number of colors in image (0 if >=8bpp)
	BYTE bReserved; // Reserved ( must be 0)
	WORD wPlanes; // Color Planes
	WORD wBitCount; // Bits per pixel
	DWORD dwBytesInRes; // How many bytes in this resource?
	DWORD dwImageOffset; // Where in the file is this image?
} ICONDIRENTRY, *LPICONDIRENTRY;
#pragma pack (1)

//icon后面的头的数据结构
typedef struct
{
	WORD idReserved; // Reserved (must be 0)
	WORD idType; // Resource Type (1 for icons)
	WORD idCount; // How many images?
	ICONDIRENTRY idEntries[1]; // An entry for each image (idCount of 'em)
} ICONDIR, *LPICONDIR;