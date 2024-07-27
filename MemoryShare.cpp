#include "StdAfx.h"
#include "MemoryShare.h"

map<CString, HANDLE> CMemoryShare::m_shareHMap;
map<CString, LPVOID> CMemoryShare::m_shareMap;

CMemoryShare::CMemoryShare(void)
{
}


CMemoryShare::~CMemoryShare(void)
{
	closeAllShare();
}

LPVOID CMemoryShare::openShare(const char *name, int nSize)
{
	// 如果已开辟共享内存直接返回
	if ((m_shareMap.find(name) != m_shareMap.end()))
	{
		return m_shareMap[name];
	}

	bool isOpen = false;
	LPVOID lpShare = nullptr;
	HANDLE hShareMem = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, name);

	if (hShareMem)
	{
		isOpen = true;
	}
	else
	{
		hShareMem = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, nSize, name);
		if ((hShareMem) && (INVALID_HANDLE_VALUE != hShareMem))
		{
			isOpen = true;
		}
	}

	if (isOpen)
	{
		lpShare = MapViewOfFile(hShareMem, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		m_shareHMap[name] = hShareMem;
		m_shareMap[name] = lpShare;
	}

	return lpShare;
}

void CMemoryShare::closeShare(const char *name)
{
	if ((m_shareMap.find(name) != m_shareMap.end()))
	{
		HANDLE lpShare = m_shareMap[name];
		if (lpShare)
		{
			UnmapViewOfFile(lpShare);
		}
		m_shareMap.erase(name);
	}

	if ((m_shareHMap.find(name) != m_shareHMap.end()))
	{
		HANDLE hShareName = m_shareHMap[name];
		if (hShareName)
		{
			CloseHandle(hShareName);
			hShareName = nullptr;
		}
		m_shareHMap.erase(name);
	}
}

void CMemoryShare::closeAllShare()
{
	if (!m_shareMap.empty())
	{
		for (auto it = m_shareMap.begin(); it != m_shareMap.end(); ++it)
		{
			if (it->second != nullptr)
			{
				UnmapViewOfFile(it->second);
			}
		}

		m_shareMap.clear();
	}

	if (!m_shareHMap.empty())
	{
		for (auto it = m_shareHMap.begin(); it != m_shareHMap.end(); ++it)
		{
			if (it->second != nullptr)
			{
				CloseHandle(it->second);
			}
		}

		m_shareHMap.clear();
	}
}
