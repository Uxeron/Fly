#include <windows.h>
#include "resource.h"
#include "fly.h"

#define TIMER_FLY 1001

HBITMAP hBitmap_Fly_1 = NULL;
HBITMAP hBitmap_Fly_2 = NULL;
HBITMAP* hBitmap_Fly_Current = &hBitmap_Fly_1;

Fly fly;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const char CLASS_NAME[]  = "Fly Transparent Window Class";

    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Create the window.
    HWND hWnd = CreateWindowEx(
        WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
        CLASS_NAME, "Fly", WS_POPUP, 500, 500, 64, 64, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL)
        return 0;

    // Set transparency color key
    SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);

    // Load images from embedded resource file
    hBitmap_Fly_1 = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_FLY_1), IMAGE_BITMAP, 0, 0, 0);
    hBitmap_Fly_2 = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_FLY_2), IMAGE_BITMAP, 0, 0, 0);

    // Start fly event timer
    SetTimer(hWnd, TIMER_FLY, 33, (TIMERPROC) NULL);

    // Start GUI
    ShowWindow(hWnd, nCmdShow);

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_PAINT:
        {
            // Draw the fly image
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            HDC hdcMem = CreateCompatibleDC(hdc);
            HGDIOBJ oldBitmap = SelectObject(hdcMem, *hBitmap_Fly_Current);

            BITMAP bitmap;
            GetObject(*hBitmap_Fly_Current, sizeof(bitmap), &bitmap);
            BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

            SelectObject(hdcMem, oldBitmap);
            DeleteDC(hdcMem);

            EndPaint(hWnd, &ps);
            break;
        }

        case WM_TIMER:
        {
            // Make sure this is the right timer
            if (wParam != TIMER_FLY)
                break;

            // Update fly image
            hBitmap_Fly_Current = hBitmap_Fly_Current == &hBitmap_Fly_1 ? &hBitmap_Fly_2 : &hBitmap_Fly_1;
            InvalidateRect(hWnd, NULL, false);

            // Move fly
            RECT window;
            GetWindowRect(hWnd, &window);

            POINT mouse;
            GetCursorPos(&mouse);

            Vector fly_target = fly.move(window.left, window.top, mouse.x, mouse.y);
            SetWindowPos(hWnd, NULL, fly_target.x, fly_target.y, 0, 0, SWP_NOSIZE);

            // Reset fly timer
            SetTimer(hWnd, TIMER_FLY, 33, (TIMERPROC) NULL);
            break;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
