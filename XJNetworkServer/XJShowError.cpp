#include "XJShowError.h"

void ShowError(DWORD dwError)
{
	HLOCAL hLocal = NULL;

	DWORD sysLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

	BOOL bRes = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, dwError, sysLocale, (PTSTR)&hLocal, 0, NULL);

	if (!bRes)
	{
		HMODULE hDll = LoadLibraryEx(TEXT("metmsg.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES);
		if (NULL != hDll)
		{
			bRes = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, hDll, dwError, sysLocale, (PTSTR)&hLocal, 0, NULL);
			FreeLibrary(hDll);
		}
	}

	if (bRes && (NULL != hLocal))
	{
		MessageBox(NULL, (PCTSTR)LocalLock(hLocal), TEXT("Error"), MB_OK | MB_ICONERROR);
		LocalFree(hLocal);
	}
	else
	{
		MessageBox(NULL, TEXT("Î´Öª´íÎó£¡"), TEXT("Error"), MB_OK | MB_ICONERROR);
	}
}