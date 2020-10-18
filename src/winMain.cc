// Interfaces with wingdiplus APIs

#include <iostream>
#include <math.h>
#include <windows.h>
#include <gdiplus/gdiplus.h>

using namespace Gdiplus;

LRESULT CALLBACK wndProc(HWND hwnd, UINT Message, 
                         WPARAM wParam, LPARAM lParam);
HDC h;
int width = 300;
int height = 300;
int nArgs;
LPWSTR *argList;

HWND hwnd;
HDC hDC;
HDC memDC;
HBITMAP memBitmap;

extern Main(int argc, char** argv);

HANDLE mainThread;
DWORD mainThreadId;

DWORD Thread(LPVOID data)
{
  char** a = (char**)argList; 
  Main(nArgs, a);
  ExitThread(0);
  return 0;
}
  

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
                   LPSTR lpCmdLine, int nCmdShow)
{
  // parse the command line
  argList =  ::CommandLineToArgvW(::GetCommandLineW(), &nArgs);

  // initialize GDI+
  GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR           gdiplusToken;
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
  
  // create window
  WNDCLASS wc = {};
  wc.lpfnWndProc = (WNDPROC)::wndProc;
  wc.hInstance = hInst;
  wc.lpszClassName = "MainWClass";
  ::RegisterClass(&wc);
  hwnd = ::CreateWindowEx(
        0,                   // optional window style
        "MainWClass",        // name of window class
        "Sample",            // title-bar string
        WS_OVERLAPPEDWINDOW, // top-level window
        CW_USEDEFAULT,       // default horizontal position
        CW_USEDEFAULT,       // default vertical position
        width,               // default width
        height,              // default height
        (HWND) NULL,         // no owner window
        (HMENU) NULL,        // use class menu
        hInst,               // handle to application instance
        (LPVOID) NULL);      // no window-creation data
  ::ShowWindow(hwnd, 1);
  ::UpdateWindow(hwnd);

  // create a memDC for objects to be drawn onto
  HDC hDC = ::GetDC(hwnd); 
  memDC = ::CreateCompatibleDC(hDC);
  memBitmap = ::CreateCompatibleBitmap(hDC, width, height);
  ::SelectObject(memDC, memBitmap);
  ::ReleaseDC(hwnd, hDC);

  // start the main thread
  mainThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread,
                              NULL, 0, &mainThreadId);

  // message loop
  MSG msg {};
  int status;
  while ((status = ::GetMessage(&msg, 0, 0, 0)) != 0) {
    if (status != -1) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
  }

  GdiplusShutdown(gdiplusToken);
  return 1;
}

LRESULT CALLBACK
wndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

  PAINTSTRUCT ps;

  switch (Message) {
    case WM_CREATE:
      std::cout << "window create" << std::endl;
      break;
    case WM_DESTROY:
      ::PostQuitMessage(0);
      std::cout << "quit" << std::endl;
      break;
    case WM_PAINT:
      h = ::BeginPaint(hwnd, &ps);   
      ::SelectObject(memDC, memBitmap);
      ::BitBlt(h, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
      ::EndPaint(hwnd, &ps);
      break;
    case WM_LBUTTONDOWN:
      ::PostThreadMessageA(mainThreadId, Message, wParam, lParam);
      break;
    case WM_RBUTTONDOWN:
      ::PostThreadMessageA(mainThreadId, Message, wParam, lParam);
      break;
    case WM_KEYDOWN:
      ::PostThreadMessageA(mainThreadId, Message, wParam, lParam);
      break;
    case WM_CHAR:
      ::PostThreadMessageA(mainThreadId, Message, wParam, lParam);
      break;
    default:
      return DefWindowProc(hwnd, Message, wParam, lParam);
  }
  return 0;
}

void winPlot(int x, int y, int red, int green, int blue) 
{
  //_b->plot(x+5, y+40, red, green, blue);
}

void winPlotLine(int x1, int y1, int x2, int y2, 
                 int red, int green, int blue)
{
  Graphics graphics(memDC);
  Pen      pen(Color(255, red, green, blue));
  graphics.DrawLine(&pen, x1, y1, x2, y2);
}

void winPlotArc(int x, int y, float r, float startAngle,
                 float sweepAngle)
{
  //::AngleArc(_hdc, x, y, (DWORD)r, startAngle, sweepAngle);
  // draw other objects
  std::cout << "copy GDI shape to memDC" << std::endl;

  HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
  SelectObject(memDC, hPen);
  MoveToEx(memDC, 100, 100, NULL);
  ::AngleArc(memDC, 100, 100, 20, 0, 360);

  HBRUSH hBrush = CreateSolidBrush(RGB(125,0,0));
  RECT rectangle = {200, 200, 250, 250};
  FillRect(memDC, &rectangle, hBrush);
}

void winPlotText(int x, int y, const char* string)
{
  Graphics graphics(memDC);
  FontFamily fontFamily(L"Arial");
  Font       font(&fontFamily, 8, FontStyleRegular, UnitPoint);
  PointF     origin(x,y);
  SolidBrush solidBrush(Color(255, 0, 0, 0));
  const size_t cSize = strlen(string) + 1;
  wchar_t* wc = new wchar_t[cSize];
  mbstowcs(wc, string, cSize);
  graphics.DrawString(wc, -1, &font, origin, NULL, &solidBrush);
}

void winClear(int red, int green, int blue)
{
  Graphics graphics(memDC);
  graphics.Clear(Color(100, red, green, blue));
}

void winFlush() 
{
  ::InvalidateRect(hwnd, 
                  NULL  /* invaliate the entire client area */, 
                  FALSE /* erase */);
}
