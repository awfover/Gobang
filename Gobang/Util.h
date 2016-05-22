#pragma once
#include "StdAfx.h"
#include <atlbase.h>

WCHAR *C2W(char *szStr) {
	WCHAR wszClassName[255];
	memset(wszClassName, 0, sizeof(wszClassName));
	MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName, sizeof(wszClassName) / sizeof(wszClassName[0]));
	return wszClassName;
}

bool GetComputerIPs() {
	// ³õÊ¼»¯
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		return false;
	}

	char hostname[255];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
		return false;
	}

	HOSTENT *lpHost = gethostbyname(hostname);
	if (lpHost == nullptr) {
		return false;
	}

	int i = 0;
	in_addr addr;
	WCHAR address[255];
	while (lpHost->h_addr_list[i] != 0) {
		addr.s_addr = *(u_long *)lpHost->h_addr_list[i++];
		wcscpy(address, C2W(inet_ntoa(addr)));
	}
}