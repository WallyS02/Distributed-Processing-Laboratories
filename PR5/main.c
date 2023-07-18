#include <string.h>
#include <windows.h>
#include <conio.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main() {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = TEXT("ClipboardViewer");
    RegisterClass(&wc);
    HWND hWnd = CreateWindowEx(
            0,
            TEXT("ClipboardViewer"),
            TEXT("Clipboard Viewer"),
            0,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            NULL,
            NULL,
            GetModuleHandle(NULL),
            NULL
    );
    if (hWnd) {
        MSG msg = { 0 };
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            AddClipboardFormatListener(hWnd);
            break;
        case WM_DESTROY:
            RemoveClipboardFormatListener(hWnd);
            PostQuitMessage(0);
            break;
        case WM_CLIPBOARDUPDATE:
            if (IsClipboardFormatAvailable(CF_TEXT)) {
                if (OpenClipboard(NULL)) {
                    HANDLE hClipboardData = GetClipboardData(CF_TEXT);
                    char* text = (char*)GlobalLock(hClipboardData);
                    if (strcmp(text, "Ala ma kota") == 0) {
                        const char* output = "Ala nie ma kota";
                        const size_t len = strlen(output) + 1;
                        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
                        if (hMem != NULL) {
                            memcpy(GlobalLock(hMem), output, len);
                            GlobalUnlock(hMem);
                            EmptyClipboard();
                            SetClipboardData(CF_TEXT, hMem);
                        }
                    }
                    GlobalUnlock(hClipboardData);
                    CloseClipboard();
                }
            }
            break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}