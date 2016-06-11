#include "Util.h"

TCHAR *M2W(char *szStr)
{
	if (szStr == nullptr)
	{
		return L"";
	}

	LPTSTR wszStr = new TCHAR[strlen(szStr) + 1];
	MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszStr, strlen(szStr) + 1);
	return wszStr;
}

char *W2M(TCHAR *wszStr)
{
	if (wszStr == nullptr)
	{
		return "";
	}

	UINT len = WideCharToMultiByte(CP_OEMCP, 0, wszStr, -1, NULL, 0, NULL, false);
	char *szStr = new char[len];
	WideCharToMultiByte(CP_OEMCP, NULL, wszStr, -1, szStr, len, NULL, FALSE);
	return szStr;
}

char *GetHostName()
{
	char *hostname = new char[255];
	if (gethostname(hostname, 255) == SOCKET_ERROR)
	{
		return nullptr;
	}
	return hostname;
}

void GetBroadcastIPList(vector<LPCTSTR> &broadcastIPList)
{
	PMIB_IPADDRTABLE pIPTable = nullptr;
	DWORD dwSize;
	GetIpAddrTable(pIPTable, &dwSize, true);

	pIPTable = new MIB_IPADDRTABLE[dwSize];
	GetIpAddrTable(pIPTable, &dwSize, true);

	broadcastIPList.clear();
	broadcastIPList.push_back(L"255.255.255.255");
	for (DWORD i = 0; i < pIPTable->dwNumEntries; i++)
	{
		if (pIPTable->table[i].dwAddr == 16777343)
		{
			continue;
		}

		int addr[] = {
			LOWORD(pIPTable->table[i].dwAddr & pIPTable->table[i].dwMask) & 0x00FF,
			LOWORD(pIPTable->table[i].dwAddr & pIPTable->table[i].dwMask) >> 8,
			HIWORD(pIPTable->table[i].dwAddr & pIPTable->table[i].dwMask) & 0x00FF,
			HIWORD(pIPTable->table[i].dwAddr & pIPTable->table[i].dwMask) >> 8
		};

		for (int j = 3; j >= 0; j--)
		{
			if (addr[j] == 0)
			{
				addr[j] = 255;
			}
			else
			{
				break;
			}
		}

		LPTSTR szIPAddr = new TCHAR[255];
		wsprintf(szIPAddr, L"%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
		broadcastIPList.push_back(szIPAddr);
	}
}

void GetComputerIPList(vector<LPCTSTR> &list)
{
	char *hostname = GetHostName();
	if (!hostname)
	{
		return;
	}

	HOSTENT *lpHost = gethostbyname(hostname);
	if (lpHost == nullptr)
	{
		return;
	}

	int i = 0;
	in_addr addr;
	LPCTSTR address = nullptr;

	list.clear();
	while (lpHost->h_addr_list[i] != 0)
	{
		memset(&addr, 0, sizeof(addr));
		addr.s_addr = *(u_long *)lpHost->h_addr_list[i++];
		list.push_back((LPCTSTR)M2W(inet_ntoa(addr)));
	}
}

LPCTSTR GetInstancePath()
{
	return CPaintManagerUI::GetInstancePath().GetData();
}
