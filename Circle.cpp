#include "framework.h"
#include "Circle.h"

#define MAX_LOADSTRING 100

//Глобальные переменные:
HINSTANCE hInst;                                // Текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // Имя класса главного окна
HBITMAP hBitmap;                                // Круг bmp
int xTop, yTop, xBottom, yBottom;
int r, d;

// Прямые объявления функций, включенных в этот модуль кода :
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

    // Инициализируем изображение
    hBitmap = (HBITMAP)LoadImage(hInstance, L"circle.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    BITMAP bm; 
    GetObject(hBitmap, sizeof(bm), &bm);

    HDC hdcBitmap = CreateCompatibleDC(nullptr);

    BITMAPINFO bmpInfo = {0};
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = bm.bmWidth;
    bmpInfo.bmiHeader.biHeight = -bm.bmHeight;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    unsigned char * pixels = new unsigned char[bm.bmWidth * bm.bmHeight * 4];
    GetDIBits(hdcBitmap, hBitmap, 0, bm.bmHeight, pixels, &bmpInfo, DIB_RGB_COLORS);
    bool found = false;
    for (int y = 0; y < bm.bmHeight; ++y)
    {
        for (int x = 0; x < bm.bmWidth; ++x)
        {
            int r = GetRValue(pixels[(x + y * bm.bmWidth) * 4 + 2]);
            int g = GetGValue(pixels[(x + y * bm.bmWidth) * 4 + 1]);
            int b = GetBValue(pixels[(x + y * bm.bmWidth) * 4 + 0]);
            if ((r <= 10) and (g <= 10) and (b <= 10))
            {
                xTop = x;
                yTop = y;
                found = true;
                break;
            }
        }
        if (found) { break; }
    }
    found = false;
    for (int y = bm.bmHeight - 1; y >= 0; --y)
    {
        for (int x = bm.bmWidth - 1; x >= 0; --x)
        {
            int r = GetRValue(pixels[(x + y * bm.bmWidth) * 4 + 2]);
            int g = GetGValue(pixels[(x + y * bm.bmWidth) * 4 + 1]);
            int b = GetBValue(pixels[(x + y * bm.bmWidth) * 4 + 0]);
            if ((r <= 10) and (g <= 10) and (b <= 10))
            {
                xBottom = x;
                yBottom = y;
                found = true;
                break;
            }
        }
        if (found) { break; }
    }
    d = (yBottom - yTop);
    r = d / 2;
    xTop += (xBottom - xTop) / 2;
    xBottom = xTop;
    xTop -= r;
    xBottom += r;
    delete[] pixels;

    // Инициализируем глобальные строки
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CIRCLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CIRCLE));

    MSG msg;

    // Основной цикл сообщений:
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
//  ФУНКЦИЯ: MyRegisterClass ()
//
//  НАЗНАЧЕНИЕ: Регистрирует класс окна.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CIRCLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CIRCLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance (HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: Сохраняет дескриптор экземпляра и создает главное окно
//
//   Комментарии:
//
//        В этой функции мы сохраняем дескриптор экземпляра в глобальной переменной и
//      создать и отобразить главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Помещаем дескриптор экземпляра в нашу глобальную переменную

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
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ: Обрабатывает сообщения для главного окна
//
//  WM_COMMAND - обработать меню приложения
//  WM_PAINT - Раскрашиваем главное окно
//  WM_DESTROY - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {        
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            //  Разберите пункты меню :
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HDC tmp = CreateCompatibleDC(hdc);
            HGDIOBJ oldBitmap = SelectObject(tmp, hBitmap);
            BITMAP bitmap;
            GetObject(hBitmap, sizeof(bitmap), &bitmap);
            RECT rect;
            GetClientRect(hWnd, &rect);
            BitBlt(hdc, rect.right - d, (rect.bottom - rect.top) / 2 - r, d, d, tmp, xTop, yTop, SRCCOPY);

            SelectObject(tmp, oldBitmap);
            DeleteDC(tmp);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        DeleteObject(hBitmap);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна About
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
