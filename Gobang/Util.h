#pragma once
#include "StdAfx.h"
#include <atlbase.h>
#include <iphlpapi.h>
#include <vector>
#pragma comment(lib, "iphlpapi.lib")

using namespace std;

TCHAR *M2W(char *);
char *W2M(TCHAR *);
char *GetHostName();
void GetBroadcastIPList(vector<LPCTSTR> &);
void GetComputerIPList(vector<LPCTSTR> &);
LPCTSTR GetInstancePath();