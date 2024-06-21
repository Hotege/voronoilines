
#include <Windows.h>
#include <tchar.h>

TCHAR CLASS_NAME[] = _T("_VORONOI_LINES_GAME_WIN32_");
TCHAR TITLE[] = _T("Voronoi Lines");
INT W = 320, H = 200;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void CenterWindow(HWND hWnd)
{
    int cx = GetSystemMetrics(SM_CXSCREEN);
    int cy = GetSystemMetrics(SM_CYSCREEN);
    RECT rt;
    GetWindowRect(hWnd, &rt);
    SetWindowPos(hWnd, NULL,
        (cx - (rt.right - rt.left)) / 2, (cy - (rt.bottom - rt.top)) / 2,
        0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASSEX wcex =
    {
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW,
        WndProc,
        0, 0,
        hInstance,
        NULL,
        (HCURSOR)LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)GetStockObject(LTGRAY_BRUSH),
        NULL,
        CLASS_NAME,
        NULL
    };
    RegisterClassEx(&wcex);
    DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT rt = { 0, 0, W, H };
    AdjustWindowRect(&rt, dwStyle, FALSE);
    HWND hWnd = CreateWindow(CLASS_NAME, TITLE, dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rt.right - rt.left, rt.bottom - rt.top,
        NULL, NULL, hInstance, NULL);
    CenterWindow(hWnd);
    UpdateWindow(hWnd);
    ShowWindow(hWnd, nCmdShow);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
