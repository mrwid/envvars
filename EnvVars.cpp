// EnvVars.cpp:
// WinMain()��Applicationʵ��
//////////////////////////////////////////////////////////////////////

#include "EnvVars.h"
#include "EnvVarsDlg.h"


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	__app.hInstance = hInstance;
	__app.nCmdShow = nCmdShow;
	App_InitCmdArguments();

	EnvVarsDlg * pMainDlg = CLS_Member( EnvVarsDlg, New )();

	int nResult = CLS_Member( EnvVarsDlg, DoModal )( pMainDlg, HWND_DESKTOP );

	CLS_Member( EnvVarsDlg, Delete )(pMainDlg);
	return 0;
}

// Application Data ------------------------------------------------------
App __app;

void App_InitCmdArguments( void )
{
	// �����в�������
	LPWSTR lpszCmdLine = GetCommandLineW();
	int nNumArgs;
	LPWSTR * pArgsArr = CommandLineToArgvW( lpszCmdLine, &nNumArgs );
	for ( int i = 0; i < nNumArgs; ++i )
	{
		__app.CmdArguments.push_back( UnicodeToString( pArgsArr[i] ) );
	}
	GlobalFree( (HGLOBAL)pArgsArr );
}
