#include "StdAfx.h"
#include "GobangManager.h"
#include "Util.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
	// CPaintManagerUI::SetResourceZip(_T("QQMusic.zip"));

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr))
	{
		return 0;
	}

	CGobangManager *lpManager = new CGobangManager();
	lpManager->ShowWelcome();

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}