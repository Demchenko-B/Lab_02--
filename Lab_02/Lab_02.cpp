// Lab_02.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab_02.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB02, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB02));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB02));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB02);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_SIZE:
        // перемальовуємо вікно при зміні розміру для динамічного центрування
        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // отримуємо поточні розміри клієнтської області
        RECT rc;
        GetClientRect(hWnd, &rc);
        int cx = rc.right / 2;  // Центр по осі X
        int cy = rc.bottom / 2; // Центр по осі Y

        // 1. малюємо осі координат (Чорний колір)
        HPEN hPenAxis = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPenAxis);

        MoveToEx(hdc, 0, cy, NULL);         // Вісь X
        LineTo(hdc, rc.right, cy);
        MoveToEx(hdc, cx, 0, NULL);         // Вісь Y
        LineTo(hdc, cx, rc.bottom);

        // нгалаштування для графіків Безьє
        const int NUM_SEGMENTS = 20; // Кількість напівперіодів
        const int NUM_POINTS = NUM_SEGMENTS * 3 + 1;
        POINT ptSin[NUM_POINTS];
        POINT ptCos[NUM_POINTS];

        int W = 120; // Довжина напівперіоду (відповідає Пі)
        int A = 100; // Амплітуда (висота хвилі)

        // починаємо малювати далеко зліва
        int startX_sin = cx - (10 * W);

        // 2. формуємо масив точок для синуса
        ptSin[0].x = startX_sin;
        ptSin[0].y = cy;
        for (int i = 0; i < NUM_SEGMENTS; i++) {
            int curX = startX_sin + i * W;
            int curSign = (i % 2 == 0) ? -1 : 1;

            ptSin[i * 3 + 1].x = curX + W / 3;
            ptSin[i * 3 + 1].y = cy + curSign * (A * 4 / 3);

            ptSin[i * 3 + 2].x = curX + 2 * W / 3;
            ptSin[i * 3 + 2].y = cy + curSign * (A * 4 / 3);

            ptSin[i * 3 + 3].x = curX + W;
            ptSin[i * 3 + 3].y = cy;
        }

        // 3. формуємо масив точок для косинуса
        int startX_cos = startX_sin - (W / 2);
        ptCos[0].x = startX_cos;
        ptCos[0].y = cy;
        for (int i = 0; i < NUM_SEGMENTS; i++) {
            int curX = startX_cos + i * W;
            int curSign = (i % 2 == 0) ? -1 : 1;

            ptCos[i * 3 + 1].x = curX + W / 3;
            ptCos[i * 3 + 1].y = cy + curSign * (A * 4 / 3);

            ptCos[i * 3 + 2].x = curX + 2 * W / 3;
            ptCos[i * 3 + 2].y = cy + curSign * (A * 4 / 3);

            ptCos[i * 3 + 3].x = curX + W;
            ptCos[i * 3 + 3].y = cy;
        }

        // 4. виводимо графік синуса (синій)
        HPEN hPenSin = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
        SelectObject(hdc, hPenSin);
        PolyBezier(hdc, ptSin, NUM_POINTS);

        // 5. виводимо графік косинуса (червоний)
        HPEN hPenCos = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
        SelectObject(hdc, hPenCos);
        PolyBezier(hdc, ptCos, NUM_POINTS);

        // звільнення ресурсів GDI
        SelectObject(hdc, hOldPen);
        DeleteObject(hPenAxis);
        DeleteObject(hPenSin);
        DeleteObject(hPenCos);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
