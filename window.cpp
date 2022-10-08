#include <windows.h>
#include "resource.h"
#include "fly.h"

#define TIMER_FLY 1001
#define WM_TRAY (WM_USER + 1)

HICON hIcon = NULL;
HBITMAP hBitmap_Fly_1 = NULL;
HBITMAP hBitmap_Fly_2 = NULL;
HBITMAP* hBitmap_Fly_Current = &hBitmap_Fly_1;

Fly fly;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddTrayIcon(HWND hWnd);
void RemoveTrayIcon(HWND hWnd);

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
    hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, 0);
    hBitmap_Fly_1 = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_FLY_1), IMAGE_BITMAP, 0, 0, 0);
    hBitmap_Fly_2 = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_FLY_2), IMAGE_BITMAP, 0, 0, 0);

    // Start fly event timer
    SetTimer(hWnd, TIMER_FLY, 33, (TIMERPROC) NULL);

    // Create the tray icon
    AddTrayIcon(hWnd);

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
            RemoveTrayIcon(hWnd);
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

        case WM_TRAY:
        {
            switch (LOWORD(lParam))
            {
                case WM_RBUTTONDOWN:
                case WM_CONTEXTMENU:
                {
                    HMENU hmenu = CreatePopupMenu();
                    InsertMenu(hmenu, 0, MF_BYPOSITION | MF_STRING, 1, "Quit");

                    SetForegroundWindow(hWnd);

                    POINT pt;
                    GetCursorPos(&pt);
                    int cmd = TrackPopupMenu(hmenu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON, pt.x, pt.y, 0, hWnd, NULL);

                    if (cmd != 0)
                        PostMessage(hWnd, WM_DESTROY, 0, 0);

                    break;
                }

                case WM_LBUTTONDBLCLK:
                {
                    PostMessage(hWnd, WM_DESTROY, 0, 0);
                    break;
                }
            }

            return 0;
        }
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void AddTrayIcon(HWND hWnd)
{
    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(nid);
    nid.hWnd = hWnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAY;
    nid.hIcon = hIcon;

    Shell_NotifyIcon(NIM_ADD, &nid);
}

void RemoveTrayIcon(HWND hWnd)
{
    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(nid);
    nid.hWnd = hWnd;
    nid.uID = 1;

    Shell_NotifyIcon(NIM_DELETE, &nid);
}
