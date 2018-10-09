#include "header.h"

BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground, HINSTANCE hInstance) {
	WNDCLASS wc; wc.style = wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.lpfnWndProc = Proc; wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(brBackground + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szName;
	return (RegisterClass(&wc) != 0);
}
