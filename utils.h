// utils.h: interface for the utils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTILS_H__CC4A0943_DFA5_46E4_BA2C_60D59EB89599__INCLUDED_)
#define AFX_UTILS_H__CC4A0943_DFA5_46E4_BA2C_60D59EB89599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )

#include <ocidl.h>
#include <vector>
#include <string>
#include <gdiplus.h>
#include <math.h>
#include "MemImage.h"
#include "MemDC.h"

// ����֧�� ---------------------------------------------------------------
typedef std::basic_string<CHAR> AnsiString;
typedef AnsiString LocalString;
typedef std::basic_string<WCHAR> UnicodeString;
typedef std::basic_string<TCHAR> String;

typedef std::vector<AnsiString> AnsiStringArray;
typedef std::vector<LocalString> LocalStringArray;
typedef std::vector<UnicodeString> UnicodeStringArray;
typedef std::vector<String> StringArray;

// ������� ---------------------------------------------------------------
bool IsExpandString( LPCTSTR lpsz );
String ExplainEnvVars( LPCTSTR lpsz );

// �ִ���� ---------------------------------------------------------------
/* ����һ���ַ������ж��ٸ��ַ�(by local CodePage),����mbstowcs */
UINT LocalCharsCount( LPCSTR lpszLocal );
/* ����һ��unicode�ַ������ٳ���(by local CodePage),����wcstombs */
UINT UnicodeMinLength( LPCWSTR lpszUnicode );
/* Unicodeת��������Ansi */
LocalString UnicodeToLocal( UnicodeString const & strUnicode );
/* ����ANSIת��Unicode */
UnicodeString LocalToUnicode( LocalString const & strLocal );

/* �����ַ�����Unicode,Local�ַ����໥ת�� */
String LocalToString( LocalString const & strLocal );
String UnicodeToString( UnicodeString const & strUnicode );
LocalString StringToLocal( String const & str );
UnicodeString StringToUnicode( String const & str );

/* UTF8����ݴ�֮���ת�� */
String UTF8ToString( AnsiString const & strUTF8 );
AnsiString StringToUTF8( String const & str );

int StrSplit( LPCTSTR lpsz, LPCTSTR lpszDelim, StringArray * pArr );
String StrJoin( LPCTSTR lpszDelim, StringArray const & arr );
String StrInsert( LPCTSTR lpsz, int iStart, int iEnd, LPCTSTR lpsz2 );
String FormatExV( int cchMsg, LPCTSTR lpszFormat, va_list args );
String FormatEx( int cchMsg, LPCTSTR lpszFormat, ... );
String Format( LPCTSTR lpszFormat, ... );

// ��Դ��� ---------------------------------------------------------------
/* �����ַ�����Դ */
UnicodeString LoadStringResExW( HMODULE hModule, UINT uStringResID );
String LoadStringResEx( HMODULE hModule, UINT uStringResID );
String LoadStringRes( UINT uStringResID );

// ϵͳ��� ---------------------------------------------------------------
/* ����ģ��·�������ģ���ļ��� */
String ModulePath( HMODULE hModule = NULL, String * pStrFileName = NULL );
/* ��ȡ·���� */
String FilePath( String const & strFull, String * pStrFileName = NULL );
/* ��ȡһ���ļ����ı������չ�� */
String FileTitle( String const & strFileName, String * pStrExtName = NULL );
/* �ж��Ƿ���һ��Ŀ¼ */
BOOL IsDirectory( LPCTSTR lpszPath );

/* ��ó���������в��� */
int GetCommandArguments( StringArray * pArr );
/* ʹ�ܹػ� */
bool ShutdownPrivilege( bool bEnable );

// ʱ����� ---------------------------------------------------------------
ULONGLONG GetUTCTimeMS( void );
UINT GetUTCTime( void );
void GetUTCSysTimeByUTCMS( ULONGLONG ullMilliseconds, LPSYSTEMTIME lpSysTime );
void GetLocalSysTimeByUTCMS( ULONGLONG ullMilliseconds, LPSYSTEMTIME lpSysTime );

// UI��� -----------------------------------------------------------------
/* ���ڶ�ʱ�� */
class WindowTimer
{
public:
	WindowTimer( void );
	void Create( HWND hWnd, UINT uElapse );
	void Destroy( void );
	UINT GetID( void ) const;
	~WindowTimer( void );
private:
	HWND _hWnd;
	UINT_PTR _uID;
	static UINT_PTR _uIDAutoIncrement;
	
	WindowTimer( WindowTimer const & );
	WindowTimer & operator = ( WindowTimer const & );
};
// ������� ---------------------------------------------------------------
/* ����һ����Ϣ�� */
void MsgBox( String const & strMsg );

/* ��ȡ�ͻ������� */
RECT Window_GetClient( HWND hWnd );
/* ��ȡ���ھ���,����ڸ����� */
RECT Window_GetRect( HWND hWnd );
/* ���ô��ھ���,����ڸ����� */
void Window_SetRect( HWND hWnd, LPCRECT lpRect );
/* ���ʹ��� */
void Window_Inflate( HWND hWnd, int dx, int dy );
/* ����Դ���Ϊ׼���д��� */
void Window_Center( HWND hWnd, HWND hRelativeWnd, bool bInRelativeWnd = false );
/* �����ı� */
String Window_GetText( HWND hWnd );
void Window_SetText( HWND hWnd, String const & str );

// ListView����
void ListView_AddStrings( HWND hListView, int nArgCount, ... );
void ListView_InsertStrings( HWND hListView, int iItemIndex, int nArgCount, ... );
void ListView_SetStrings( HWND hListView, int iItemIndex, int nArgCount, ... );
void ListView_GetStrings( HWND hListView, int iItemIndex, int nArgPairCount, ... );


#endif // !defined(AFX_UTILS_H__CC4A0943_DFA5_46E4_BA2C_60D59EB89599__INCLUDED_)
