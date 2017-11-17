#pragma once

#ifdef XJSERVICESLIB_EXPORTS
#define DLL_PORT __declspec(dllexport)
#else 
#define DLL_PORT __declspec(dllimport)
#endif

class IXJUnknown
{
public:
	virtual void AddRef() = 0;
	virtual void Release() = 0;
	virtual void QueryInterface(GUID GIID, void **pInterface) = 0;
};