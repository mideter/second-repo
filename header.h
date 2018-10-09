#pragma once

#define UNICODE
#define _UNICODE

#include <Windows.h>
#include <tchar.h>
#include <string>
#include <sstream>
#include <windowsx.h>
using std::wstring;
using std::to_wstring;

#define ID_BUTTON 3000
#define ID_EDIT 3010
#define ID_EDIT_RES 3011

BOOL RegClass(WNDPROC, LPCTSTR, UINT, HINSTANCE);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID Dlg_OnCommand();
VOID AddString_EDIT_RES(wstring str);
DWORD WINAPI ThreadFunc(PVOID);
void MakeTree(wstring path);
void ShowDirection(WIN32_FIND_DATAW * f, int lev);




