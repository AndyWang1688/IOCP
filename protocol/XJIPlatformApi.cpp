#include "XJIPlatformApi.h"

PFNPLATFORMDATAPROC IPlatformApi::m_pPlatformDataProc = NULL;
IPlatformApi::IPlatformApi(PFNPLATFORMDATAPROC pFn) 
{
	m_pPlatformDataProc = pFn;
}

IPlatformApi::~IPlatformApi() {}