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

// 检测一个以 null 结尾的字符串是否是UTF-8, 如果返回0, 也只表示这个串刚好符合UTF8的编码规则.
// 返回值说明: 
// 1 -> 输入字符串符合UTF-8编码规则
// -1 -> 检测到非法的UTF-8编码首字节
// -2 -> 检测到非法的UTF-8字节编码的后续字节.
int IsTextUTF8(const char* pszSrc); 
