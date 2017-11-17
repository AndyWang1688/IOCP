#pragma once
#include "IXJUnknown.h"

extern "C"
{
	DLL_PORT bool InitLoadLibrary();
	DLL_PORT IXJUnknown* CreateComObject(const char *pszFileName, GUID GMID, GUID GCID);
	DLL_PORT bool UnLoadLibrary(GUID GMID);
};