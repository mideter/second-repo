#include "header.h"

HINSTANCE hInstance;
TCHAR szClassName[] = _T("Example21");
static HWND hButton, hEdit, hEditRes;
HWND hWnd;

DWORD ThreadId;

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance,
	LPTSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;  hInstance = hInst;
	
	if (!RegClass(WndProc, szClassName, COLOR_WINDOW, hInstance))
		return FALSE;
	
	hWnd = CreateWindow(szClassName, _T("Приложение с кнопкой"),
		WS_OVERLAPPEDWINDOW,
		500, 100, 500,
		500, 0, 0, hInstance, NULL);
	
	if (!hWnd) 
		return FALSE;
	
	ShowWindow(hWnd, nCmdShow); UpdateWindow(hWnd);

	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam,
	LPARAM lParam)
{
	switch (msg) {
	case WM_CREATE:
	{
		hButton = CreateWindow(_T("button"), _T("Создать дерево"),
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 0,
			150, 20, hwnd, (HMENU)ID_BUTTON, hInstance, NULL);
		hEdit = CreateWindow(_T("edit"), _T("C:\\Users\\midet\\Desktop\\root"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			170, 0, 300, 20, hwnd, (HMENU)ID_EDIT, hInstance, NULL);
		hEditRes = CreateWindow(_T("edit"), _T("Дерево"), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOVSCROLL | ES_MULTILINE,
			10, 30, 460, 120, hwnd, (HMENU)ID_EDIT_RES, hInstance, NULL);
		return 0;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_BUTTON:
		{
			if (HIWORD(wParam) == BN_CLICKED) {
				Dlg_OnCommand();
			}
			break;
		}
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


VOID Dlg_OnCommand() {
	DWORD cb = (Edit_GetTextLength(hEdit) + 1) * sizeof(wchar_t);
	static TCHAR tstr[1000];
	Edit_GetText(hEdit, tstr, cb);
	Edit_SetText(hEditRes, L"");
	//MessageBox(hWnd, tstr, L"Button act", MB_OK);
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, tstr, 0, &ThreadId);
	CloseHandle(hThread);
}

VOID AddString_EDIT_RES(wstring str) {
	DWORD cb = (Edit_GetTextLength(hEditRes) + 1) * sizeof(wchar_t);
	TCHAR tstr[1000];
	Edit_GetText(hEditRes, tstr, cb);
	std::wstringstream ws;
	ws << tstr << str << L"\r\n";
	wstring newstr = ws.str();
	Edit_SetText(hEditRes, newstr.c_str());
}

DWORD WINAPI ThreadFunc(PVOID pvParam) {

	//MessageBox(hWnd, (TCHAR*)pvParam, L"ThreadFunc", MB_OK);
	MakeTree((TCHAR*)pvParam);
	return(0);
}

wstring foo(wstring path, WIN32_FIND_DATAW f)
{
	if (path.substr(path.size() - 3, 3) == L"*.*") {
		wstring str = path.substr(0, path.size() - 3);
		str += f.cFileName;
		str += L"\\*.*";
		return str;
	}
	else
		return path + L"\\*.*";
}

bool IsPrevOrNowCatalog(WIN32_FIND_DATA f) {
	if (wstring(f.cFileName) == L"." || wstring(f.cFileName) == L"..")
		return true;
	else
		return false;
}

void MakeTree(wstring path)
{
	static int lev = 0;
	WIN32_FIND_DATA lpFindFileData;	// информация о найденных файлах

	LPCWSTR lpcPath = (LPCWSTR)path.c_str();
	HANDLE hFindFile = FindFirstFile(lpcPath, &lpFindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		DWORD error = GetLastError();
		if (error != 0 && error != ERROR_NO_MORE_FILES)
		{
			wstring str = L"FindNextFile error. Error is " + to_wstring(error);
			MessageBox(hWnd, str.c_str(), L"Error", MB_OK);
			return;
		}
		return;
	}

	do
	{
		if(IsPrevOrNowCatalog(lpFindFileData))
			continue;
		ShowDirection(&lpFindFileData, lev);
		
		if (lpFindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			lev++;
			wstring buf = path;
			buf = foo(path, lpFindFileData);
			MakeTree(buf);
			lev--;
		}
	} while (FindNextFile(hFindFile, &lpFindFileData));
	
	FindClose(hFindFile);
}

void ShowDirection(WIN32_FIND_DATAW * f, int lev)
{
	wstring str;
	if (!(f->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
		str = L"<F>";
		for (int i = 0; i < lev; i++)
			str += L"   ";
		str += f->cFileName;
		AddString_EDIT_RES(str);
	}
	
	if ((f->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
		str = L"<D>";
		for (int i = 0; i < lev; i++)
			str += L"   ";
		str += f->cFileName;
		AddString_EDIT_RES(str);
	}

	return;
}