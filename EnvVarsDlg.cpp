// EnvVarsDlg.cpp:
//
//////////////////////////////////////////////////////////////////////
#include "EnvVars.h"
#include "EnvVarsDlg.h"
#include "RegOperate3.h"
#include "resource.h"
#include "ModifyDlg.h"
#include "SubclassListBox.h"

DLG_DeclMap(EnvVarsDlg);

CLS_New_Impl(EnvVarsDlg)
CLS_Delete_Impl(EnvVarsDlg)
DLG_DoModal_Impl( EnvVarsDlg, IDD_ENVVARS_DIALOG )

// ���뻷������
void EnvVarsDlg_LoadEnvVars( EnvVarsDlg * This )
{
	UINT const BUF_SIZE = 32767;
	TCHAR sz[BUF_SIZE];
	DWORD dwSize;
	String strTitle;
	This->strSysEnvKey = LoadStringRes(IDS_SYSENVKEY);
	This->strUserEnvKey = LoadStringRes(IDS_USERENVKEY);
	This->strVarName = LoadStringRes(IDS_VARNAME);
	if ( __app.CmdArguments.size() > 1 )
	{
		This->strVarName = __app.CmdArguments[1];
	}
	strTitle = Window_GetText(This->hDlg);
	Window_SetText( This->hDlg, ( This->strVarName + strTitle ) );
	// ���ر���
	HKEY hSysEnvKey = reg_open_key( This->strSysEnvKey.c_str(), FALSE );
	dwSize = BUF_SIZE;
	ZeroMemory( sz, dwSize );
	reg_read_ex( hSysEnvKey, This->strVarName.c_str(), (LPBYTE)sz, &This->dwRegTypeSysVar, &dwSize );
	This->sysVars.clear();
	StrSplit( sz, TEXT(";"), &This->sysVars );
	reg_close_key(hSysEnvKey);
	
	HKEY hUserEnvKey = reg_open_key( This->strUserEnvKey.c_str(), FALSE );
	dwSize = BUF_SIZE;
	ZeroMemory( sz, dwSize );
	reg_read_ex( hUserEnvKey, This->strVarName.c_str(), (LPBYTE)sz, &This->dwRegTypeUserVar, &dwSize );
	This->userVars.clear();
	StrSplit( sz, TEXT(";"), &This->userVars );
	reg_close_key(hUserEnvKey);
}

// �ύ����
void EnvVarsDlg_CommitEnvVars( EnvVarsDlg * This, String const & strEnvKey, String const & strVarName, String const & strValue, DWORD dwRegType )
{
	if ( strValue.empty() )
	{
		reg_delete( strEnvKey.c_str(), strVarName.c_str() );
	}
	else
	{
		bool bIsExpand = IsExpandString( strValue.c_str() );
		HKEY hKey = reg_open_key( strEnvKey.c_str(), FALSE );
		reg_write_ex( hKey, strVarName.c_str(), bIsExpand ? REG_EXPAND_SZ : dwRegType, (BYTE const *)strValue.c_str(), strValue.length() * sizeof(TCHAR) );
		reg_close_key(hKey);
	}
}

// ���±����б�
void EnvVarsDlg_UpdateVarsList( EnvVarsDlg * This, UINT uListBoxID, StringArray & arrVars )
{
	HWND hListBox = GetDlgItem( This->hDlg, uListBoxID );
	int i;
	SendMessage( hListBox, LB_RESETCONTENT, 0, 0 );
	for ( i = 0; i < arrVars.size(); i++ )
	{
		SendMessage( hListBox, LB_ADDSTRING, 0, (LPARAM)ExplainEnvVars( arrVars[i].c_str() ).c_str() );
	}
}

// ���ֵ
void EnvVarsDlg_OnAddVal( EnvVarsDlg * This, UINT uListBoxID, bool bIsUser, StringArray & arrVars, String const & strEnvKey, String const & strVarName )
{
	ModifyDlg * pModDlg = ModifyDlg_New();
	pModDlg->bIsUser = bIsUser;
	pModDlg->bIsAdd = true;
	if ( ModifyDlg_DoModal( pModDlg, This->hDlg ) == IDOK )
	{
		if ( !pModDlg->strValue.empty() )
		{
			arrVars.push_back( pModDlg->strValue.c_str() );
			int iSel = arrVars.size() - 1;
			EnvVarsDlg_UpdateVarsList( This, uListBoxID, arrVars );
			HWND hListBox = GetDlgItem( This->hDlg, uListBoxID );
			SendMessage( hListBox, LB_SETCURSEL, iSel, 0 );
			//EnvVarsDlg_CommitEnvVars( This, strEnvKey, strVarName, StrJoin( TEXT(";"), arrVars ), bIsUser ? This->dwRegTypeUserVar : This->dwRegTypeSysVar );
			bIsUser ? This->bUserVarHasModified = true : This->bSysVarHasModified = true;
		}
	}
	ModifyDlg_Delete(pModDlg);
}

// �޸�ֵ
void EnvVarsDlg_OnModVal( EnvVarsDlg * This, UINT uListBoxID, bool bIsUser, StringArray & arrVars, String const & strEnvKey, String const & strVarName )
{
	HWND hListBox = GetDlgItem( This->hDlg, uListBoxID );
	int iSel = SendMessage( hListBox, LB_GETCURSEL, 0, 0 );
	if ( iSel == -1 ) return;
	ModifyDlg * pModDlg = ModifyDlg_New();
	pModDlg->bIsUser = bIsUser;
	pModDlg->bIsAdd = false;
	pModDlg->strValue = arrVars[iSel].c_str();
	if ( ModifyDlg_DoModal( pModDlg, This->hDlg ) == IDOK )
	{
		arrVars[iSel] = pModDlg->strValue.c_str();
		bool bIsEmpty = arrVars[iSel].empty();
		if ( bIsEmpty ) arrVars.erase( arrVars.begin() + iSel );
		EnvVarsDlg_UpdateVarsList( This, uListBoxID, arrVars );
		SendMessage( hListBox, LB_SETCURSEL, bIsEmpty ? iSel - 1 : iSel, 0 );
		//EnvVarsDlg_CommitEnvVars( This, strEnvKey, strVarName, StrJoin( TEXT(";"), arrVars ), bIsUser ? This->dwRegTypeUserVar : This->dwRegTypeSysVar );
		bIsUser ? This->bUserVarHasModified = true : This->bSysVarHasModified = true;
	}
	ModifyDlg_Delete(pModDlg);
}

// �Ի����ʼ����Ϣ
void EnvVarsDlg_OnInitDialog( EnvVarsDlg * This )
{
	This->bUserVarHasModified = false;
	This->bSysVarHasModified = false;
	This->iDragSource = -1;
	// ͼ������
	This->hIcon = LoadIcon( __app.hInstance, MAKEINTRESOURCE(IDR_MAINFRAME) );
	SendMessage( This->hDlg, WM_SETICON, ICON_BIG, (LPARAM)This->hIcon );
	SendMessage( This->hDlg, WM_SETICON, ICON_SMALL, (LPARAM)This->hIcon );

	// λ�þ���
	Window_Center( This->hDlg, HWND_DESKTOP );

	// ���뻷������
	EnvVarsDlg_LoadEnvVars(This);

	// ���±����б�
	EnvVarsDlg_UpdateVarsList( This, IDC_LIST_USER, This->userVars );
	EnvVarsDlg_UpdateVarsList( This, IDC_LIST_SYS, This->sysVars );

	SubclassSysVarsListBox( GetDlgItem( This->hDlg, IDC_LIST_SYS ), true );
	SubclassUsrVarsListBox( GetDlgItem( This->hDlg, IDC_LIST_USER ), true );

	BOOL b;
	b = MakeDragList( GetDlgItem( This->hDlg, IDC_LIST_USER ) );
	b = MakeDragList( GetDlgItem( This->hDlg, IDC_LIST_SYS ) );
	This->uDragListMsg = RegisterWindowMessage(DRAGLISTMSGSTRING);
}

void EnvVarsDlg_OnDestroy( EnvVarsDlg * This )
{
	DLG_WndMap(EnvVarsDlg).erase(This->hDlg);
}

void EnvVarsDlg_OnOK( EnvVarsDlg * This )
{
	// �ύ���»�������
	if ( This->bUserVarHasModified )
	{
		EnvVarsDlg_CommitEnvVars( This, This->strUserEnvKey, This->strVarName, StrJoin( TEXT(";"), This->userVars ), This->dwRegTypeUserVar );
	}

	if ( This->bSysVarHasModified )
	{
		EnvVarsDlg_CommitEnvVars( This, This->strSysEnvKey, This->strVarName, StrJoin( TEXT(";"), This->sysVars ), This->dwRegTypeSysVar );
	}

	// ������Ϣ������Ӧ�ó���˵�����������Ѹı�
	if ( This->bUserVarHasModified || This->bSysVarHasModified )
	{
		LRESULT lResult;
		lResult = SendMessage( HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)TEXT("Environment") );
		//DWORD dw;
		//lResult = SendMessageTimeout( HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)TEXT("Environment"), SMTO_NORMAL, 3000, &dw );
	}
	EndDialog( This->hDlg, IDOK );
}


INT_PTR CALLBACK EnvVarsDlg_Proc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	EnvVarsDlg * This = DLG_This(EnvVarsDlg);
	if ( This != NULL && uMsg == This->uDragListMsg )
	{
		// �Ϸ��б���
		LPDRAGLISTINFO pDragListInfo = (LPDRAGLISTINFO)lParam;
		switch ( pDragListInfo->uNotification )
		{
		case DL_BEGINDRAG:
			{
				// ��ȡ�Ϸŵ�Դ�б���
				This->iDragSource = LBItemFromPt( pDragListInfo->hWnd, pDragListInfo->ptCursor, TRUE );
				// ��һ���Ի��������,��������������Ϣ����ֵ
				SetWindowLong( hDlg, DWL_MSGRESULT, TRUE );
				return TRUE;
			}
			break;
		case DL_DRAGGING:
			// ���Ϸ�Ŀ����
			DrawInsert( hDlg, pDragListInfo->hWnd, LBItemFromPt( pDragListInfo->hWnd, pDragListInfo->ptCursor, TRUE ) );
			break;
		case DL_CANCELDRAG:
			// ȡ���Ϸ�,ȡ�������ʾ
			DrawInsert( hDlg, pDragListInfo->hWnd, -1 );
			break;
		case DL_DROPPED:
			{
				// ���õ�Ŀ���б���
				int iDropTarget = LBItemFromPt( pDragListInfo->hWnd, pDragListInfo->ptCursor, TRUE );
				if ( iDropTarget != -1 && This->iDragSource != -1 && iDropTarget != This->iDragSource )
				{
					switch ( wParam )
					{
					case IDC_LIST_USER:
						{
							// �Ϸ�λ�����ݸ���
							String strVal = This->userVars[This->iDragSource];
							This->userVars.erase( This->userVars.begin() + This->iDragSource );
							This->userVars.insert( This->userVars.begin() + iDropTarget, strVal );
							EnvVarsDlg_UpdateVarsList( This, IDC_LIST_USER, This->userVars );
							SendMessage( pDragListInfo->hWnd, LB_SETCURSEL, iDropTarget, 0 );
							This->bUserVarHasModified = true;
						}
						break;
					case IDC_LIST_SYS:
						{
							String strVal = This->sysVars[This->iDragSource];
							This->sysVars.erase( This->sysVars.begin() + This->iDragSource );
							This->sysVars.insert( This->sysVars.begin() + iDropTarget, strVal );
							EnvVarsDlg_UpdateVarsList( This, IDC_LIST_SYS, This->sysVars );
							SendMessage( pDragListInfo->hWnd, LB_SETCURSEL, iDropTarget, 0 );
							This->bSysVarHasModified = true;
						}
						break;
					}
				}
				DrawInsert( hDlg, pDragListInfo->hWnd, -1 );
			}
			break;
		}
		
	}
	else
	BEGIN_MSG()
		ON_MSG(WM_INITDIALOG)
			DLG_BindHWND(EnvVarsDlg);
			EnvVarsDlg_OnInitDialog( GetPtr( EnvVarsDlg, lParam ) );
		BEGIN_CMD()
			BEGIN_EVENT(IDC_LIST_USER)
				ON_EVENT(LBN_DBLCLK)
					EnvVarsDlg_OnModVal( This, IDC_LIST_USER, true, This->userVars, This->strUserEnvKey, This->strVarName );
			END_EVENT()
			BEGIN_EVENT(IDC_LIST_SYS)
				ON_EVENT(LBN_DBLCLK)
					EnvVarsDlg_OnModVal( This, IDC_LIST_SYS, false, This->sysVars, This->strSysEnvKey, This->strVarName );
			END_EVENT()
			ON_ID(IDCANCEL)
				EndDialog( hDlg, IDCANCEL );
			ON_ID(IDOK)
				EnvVarsDlg_OnOK( DLG_This(EnvVarsDlg) );
			ON_ID(IDM_USR_ADD_VAL)
				EnvVarsDlg_OnAddVal( This, IDC_LIST_USER, true, This->userVars, This->strUserEnvKey, This->strVarName );
			ON_ID(IDM_USR_MOD_VAL)
				EnvVarsDlg_OnModVal( This, IDC_LIST_USER, true, This->userVars, This->strUserEnvKey, This->strVarName );
			ON_ID(IDM_SYS_ADD_VAL)
				EnvVarsDlg_OnAddVal( This, IDC_LIST_SYS, false, This->sysVars, This->strSysEnvKey, This->strVarName );
			ON_ID(IDM_SYS_MOD_VAL)
				EnvVarsDlg_OnModVal( This, IDC_LIST_SYS, false, This->sysVars, This->strSysEnvKey, This->strVarName );
		END_CMD()
		ON_MSG(WM_DESTROY)
			EnvVarsDlg_OnDestroy( DLG_This(EnvVarsDlg) );
	END_MSG()
	return 0;
}
