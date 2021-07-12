#pragma once

#ifdef _R3
#include <Windows.h>
#else
#include <ntifs.h>
#endif // !_R3

#define COMM_ID 0x5858
#define SYM_NAME L"\\??\\Airshelf"

typedef enum _CMD
{
	TEST = 0,
}CMD;

typedef struct _CommPackage
{
	ULONG64 id;
	ULONG64 code;
	ULONG64 inData;
	ULONG64 inLen;
	ULONG64 outData;
	ULONG64 outLen;
}CommPackage, * PCommPackage;

typedef struct _Test
{
	char *myStr;
}Test, * PTest;