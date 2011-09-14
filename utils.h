// utils.h: interface for the utils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTILS_H__CC4A0943_DFA5_46E4_BA2C_60D59EB89599__INCLUDED_)
#define AFX_UTILS_H__CC4A0943_DFA5_46E4_BA2C_60D59EB89599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>
// ����֧�� ---------------------------------------------------------------
typedef std::string AnsiString;
typedef AnsiString LocalString;
typedef std::wstring UnicodeString;

#ifndef USE_TCHAR_DECLARE
	#ifdef UNICODE
		typedef UnicodeString String;
	#else
		typedef LocalString String;
	#endif
#else
	typedef std::basic_string<TCHAR> String;
#endif

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
String OutputExV( int cchMsg, LPCTSTR lpszFormat, va_list args );
String OutputEx( int cchMsg, LPCTSTR lpszFormat, ... );
String Output( LPCTSTR lpszFormat, ... );

// ��Դ��� ---------------------------------------------------------------
/* �����ַ�����Դ */
UnicodeString LoadStringResExW( HMODULE hModule, UINT uStringResID );
String LoadStringResEx( HMODULE hModule, UINT uStringResID );
String LoadStringRes( UINT uStringResID );

// ϵͳ��� ---------------------------------------------------------------
/** ʹ�ܹػ� */
bool ShutdownPrivilege( bool bEnable );

ULONGLONG GetUTCTimeMS( void );
UINT GetUTCTime( void );
void GetUTCSysTimeByUTCMS( ULONGLONG ullMilliseconds, LPSYSTEMTIME lpSysTime );
void GetLocalSysTimeByUTCMS( ULONGLONG ullMilliseconds, LPSYSTEMTIME lpSysTime );

// UI��� -----------------------------------------------------------------
// �������

/* ����Դ���Ϊ׼���д��� */
void Window_Center( HWND hWnd, HWND hRelativeWnd );
/* �����ı� */
String Window_GetText( HWND hWnd );
void Window_SetText( HWND hWnd, String const & str );

// ListView����
void ListView_AddStrings( HWND hListView, int nArgCount, ... );
void ListView_InsertStrings( HWND hListView, int iItemIndex, int nArgCount, ... );
void ListView_SetStrings( HWND hListView, int iItemIndex, int nArgCount, ... );
void ListView_GetStrings( HWND hListView, int iItemIndex, int nArgPairCount, ... );

// ͼ��ͼ����� -----------------------------------------------------------
BOOL SaveBitmapToFile( HBITMAP hBitmap, LPSTR lpszFileName );


#endif // !defined(AFX_UTILS_H__CC4A0943_DFA5_46E4_BA2C_60D59EB89599__INCLUDED_)
