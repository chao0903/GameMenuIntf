#pragma once

#include <windows.h>
#include <string>

#if defined(_UNICODE) || defined(UNICODE)
#define TtoA WtoA
#define AtoT AtoW
#define WtoT(a) (a)
#define TtoW(a) (a)
typedef std::wstring _tstring;
#else
#define TtoA(a) (a)
#define AtoT(a) (a)
#define WtoT WtoA
#define TtoW AtoW
typedef std::string _tstring;
#endif

std::string WtoA(const wchar_t* pwszSrc);
std::string WtoA(const std::wstring &strSrc);

std::wstring AtoW(const char* pszSrc);
std::wstring AtoW(const std::string &strSrc);

std::string WtoUTF8(const wchar_t* pwszSrc);
std::string WtoUTF8(const std::wstring &strSrc);

std::wstring UTF8toW(const char* pszSrc);
std::wstring UTF8toW(const std::string &strSr);

std::string AtoUTF8(const char* src);
std::string AtoUTF8(const std::string &src);

std::string UTF8toA(const char* src);
std::string UTF8toA(const std::string &src);
std::string WStringToString(const std::wstring &wstr);

// ���һ���� null ��β���ַ����Ƿ���UTF-8, �������0, Ҳֻ��ʾ������պ÷���UTF8�ı������.
// ����ֵ˵��: 
// 1 -> �����ַ�������UTF-8�������
// -1 -> ��⵽�Ƿ���UTF-8�������ֽ�
// -2 -> ��⵽�Ƿ���UTF-8�ֽڱ���ĺ����ֽ�.
int IsTextUTF8(const char* pszSrc); 
