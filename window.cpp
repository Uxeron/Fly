#include <windows.h>
#include "resource.h"
#include "Flies\Black\fly_black.h"
#include "Flies\Red\fly_red.h"
#include "Flies\Yellow\fly_yellow.h"
#include "Flies\White\fly_white.h"

#define TIMER_FLY_ANIMATION 1001
#define TIMER_FLY_MOVE 1002
#define WM_TRAY (WM_USER + 1)

HICON hIcon = NULL;

const int flyCount = 4;
int currentAnimation = 0;
int currentFrame = 0;
HBITMAP hBitmapFlyBitmaps[6];
HBITMAP* hBitmapFlyMap[flyCount][2];

FlyBlack flyBlack;
FlyRed flyRed;
FlyYellow flyYellow;
FlyWhite flyWhite;


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddTrayIcon(HWND hWnd);
void RemoveTrayIcon(HWND hWnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"Fly Transparent Window Class";

    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Create the window.
    HWND hWnd = CreateWindowEx(
        WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
        CLASS_NAME, L"Fly", WS_POPUP, 500, 500, 64, 64, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL)
        return 0;

    // Set transparency color key
    SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);

    // Load images from embedded resource file
    hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, 0);
    hBitmapFlyBitmaps[0] = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_FLY_1), IMAGE_BITMAP, 0, 0, 0);
    hBitmapFlyBitmaps[1] = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_FLY_2), IMAGE_BITMAP, 0, 0, 0);
    hBitmapFlyBitmaps[2] = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_FLY_3), IMAGE_BITMAP, 0, 0, 0);
    hBitmapFlyBitmaps[3] = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_FLY_4), IMAGE_BITMAP, 0, 0, 0);
    hBitmapFlyBitmaps[4] = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_FLY_5), IMAGE_BITMAP, 0, 0, 0);
    hBitmapFlyBitmaps[5] = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP_FLY_6), IMAGE_BITMAP, 0, 0, 0);

    // Map fly images to animations
    hBitmapFlyMap[0][0] = &hBitmapFlyBitmaps[0];
    hBitmapFlyMap[0][1] = &hBitmapFlyBitmaps[1];
    hBitmapFlyMap[1][0] = &hBitmapFlyBitmaps[0];
    hBitmapFlyMap[1][1] = &hBitmapFlyBitmaps[2];
    hBitmapFlyMap[2][0] = &hBitmapFlyBitmaps[0];
    hBitmapFlyMap[2][1] = &hBitmapFlyBitmaps[3];
    hBitmapFlyMap[3][0] = &hBitmapFlyBitmaps[4];
    hBitmapFlyMap[3][1] = &hBitmapFlyBitmaps[5];

    // Start fly event timers
    SetTimer(hWnd, TIMER_FLY_ANIMATION, 33, (TIMERPROC) NULL); // 30 FPS
    SetTimer(hWnd, TIMER_FLY_MOVE, 16, (TIMERPROC) NULL); // 60FPS

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
            HGDIOBJ oldBitmap = SelectObject(hdcMem, *hBitmapFlyMap[currentAnimation][currentFrame]);

            BITMAP bitmap;
            GetObject(*hBitmapFlyMap[currentAnimation][currentFrame], sizeof(bitmap), &bitmap);
            BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

            SelectObject(hdcMem, oldBitmap);
            DeleteDC(hdcMem);

            EndPaint(hWnd, &ps);
            break;
        }

        case WM_TIMER:
        {
            if (wParam == TIMER_FLY_ANIMATION)
            {
                // Update fly image
                currentFrame = currentFrame == 0 ? 1 : 0;
                InvalidateRect(hWnd, NULL, false);

                // Reset fly timer
                SetTimer(hWnd, TIMER_FLY_ANIMATION, 33, (TIMERPROC) NULL);
                break;
            }
            else if (wParam == TIMER_FLY_MOVE)
            {
                // Move fly
                RECT window;
                GetWindowRect(hWnd, &window);

                POINT mouse;
                GetCursorPos(&mouse);

                Vector fly_target;

                switch(currentAnimation) 
                {
                    case 0:
                        fly_target = flyBlack.move(window.left, window.top, mouse.x, mouse.y);
                        break;
                    case 1:
                        fly_target = flyRed.move(window.left, window.top, mouse.x, mouse.y);
                        break;
                    case 2:
                        fly_target = flyYellow.move(window.left, window.top, mouse.x, mouse.y);
                        break;
                    case 3:
                        fly_target = flyWhite.move(window.left, window.top, mouse.x, mouse.y);
                        break;
                }

                SetWindowPos(hWnd, NULL, fly_target.x, fly_target.y, 0, 0, SWP_NOSIZE);

                // Reset fly timer
                SetTimer(hWnd, TIMER_FLY_MOVE, 16, (TIMERPROC) NULL);
                break;
            }
        }

        case WM_TRAY:
        {
            switch (LOWORD(lParam))
            {
                case WM_RBUTTONDOWN:
                case WM_CONTEXTMENU:
                {
                    HMENU hmenu = CreatePopupMenu();
                    InsertMenu(hmenu, 0, MF_BYPOSITION | MF_STRING, 1, L"Switch fly");
                    InsertMenu(hmenu, 1, MF_BYPOSITION | MF_STRING, 2, L"Quit");

                    SetForegroundWindow(hWnd);

                    POINT pt;
                    GetCursorPos(&pt);
                    int cmd = TrackPopupMenu(hmenu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON, pt.x, pt.y, 0, hWnd, NULL);

                    if (cmd == 1)
                        currentAnimation = ++currentAnimation >= flyCount ? 0 : currentAnimation;
                    if (cmd == 2)
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
    wcscpy(nid.szTip, L"Fly");

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
