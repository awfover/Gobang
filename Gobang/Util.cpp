#include "Util.h"

WCHAR *C2W(char *szStr) {
	LPWSTR wszClassName = new WCHAR[strlen(szStr) + 1];
	MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName, strlen(szStr) + 1);
	return wszClassName;
}

LPCTSTR GetComputerIPs() {
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
	LPCTSTR address;
	while (lpHost->h_addr_list[i] != 0) {
		addr.s_addr = *(u_long *)lpHost->h_addr_list[i++];
		address = (LPCTSTR)C2W(inet_ntoa(addr));
	}
	return address;
}