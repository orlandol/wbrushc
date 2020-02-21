
#include "windows.h"
#include "shellapi.h"

#include "sysw32.h"

/*
 *  =================================
 *  Win32 implementation declarations
 *  =================================
 */
  HINSTANCE sysw32Instance = NULL;
  HINSTANCE sysw32PrevInstance = NULL;
  int sysw32CmdShow = SW_SHOWNORMAL;

  HWND sysw32AppWindow = NULL;

  int sysArgc = 0;
  wchar_t** sysArgvw = NULL;

/*
 *  ======================
 *  Win32 helper functions
 *  ======================
 */

/*
 *  bool GetDefaultScreenResolution( unsigned* width, unsigned* height,
 *    unsigned* bpp )
 *
 *  Returns the screen resolution of the default/primary Desktop.
 */
  bool GetDefaultScreenResolution( unsigned* width, unsigned* height,
      unsigned* bpp )
  {
    MONITORINFO monitorInfo;
    POINT zeroXY;
    HMONITOR    defaultMonitor;
    HDC screenDC;
    unsigned calculatedWidth;
    unsigned calculatedHeight;
    unsigned calculatedBPP;

    // Check Width and Height to determine Windowed/Fullscreen Mode
    ZeroMemory( &zeroXY, sizeof(zeroXY) );
    defaultMonitor =
        MonitorFromPoint(zeroXY, MONITOR_DEFAULTTOPRIMARY);

    // Attempt to get monitor data
    ZeroMemory( &monitorInfo, sizeof(monitorInfo) );
    monitorInfo.cbSize = sizeof(monitorInfo);
    if( GetMonitorInfo(defaultMonitor, &monitorInfo) == 0 )
    {
      return false;
    }
    if( width )
    {
      calculatedWidth  = abs(monitorInfo.rcMonitor.right
          - monitorInfo.rcMonitor.left);
      *width = calculatedWidth;
    }

    if( height )
    {
      calculatedHeight = abs(monitorInfo.rcMonitor.bottom
          - monitorInfo.rcMonitor.top);
      *height = calculatedHeight;
    }

    if( bpp )
    {
      ///TODO: Get Desktop BPP The Correct Way (TM)
      ///TODO: Distinguish between 15/16-BPP
      screenDC = GetDC(0);
      if( screenDC )
      {
        calculatedBPP = GetDeviceCaps(screenDC, PLANES)
            * GetDeviceCaps(screenDC, BITSPIXEL);
        *bpp = calculatedBPP;
        ReleaseDC(0, screenDC);
      }
    }

    return true;
  }

/*
 *  bool GetDefaultDesktopResolution( unsigned* width, unsigned* height,
 *    unsigned* bpp )
 *
 *  Returns the desktop resolution of the default/primary Desktop.
 *    Excludes the taskbar area.
 */
  bool GetDefaultDesktopResolution( unsigned* width, unsigned* height,
      unsigned* bpp )
  {
    MONITORINFO monitorInfo;
    POINT zeroXY;
    HMONITOR defaultMonitor;
    HDC screenDC;
    unsigned calculatedWidth;
    unsigned calculatedHeight;
    unsigned calculatedBPP;

    ///TODO: Factor in the taskbar

    // Check Width and Height to determine Windowed/Fullscreen Mode
    ZeroMemory( &zeroXY, sizeof(zeroXY) );
    defaultMonitor =
        MonitorFromPoint(zeroXY, MONITOR_DEFAULTTOPRIMARY);

    // Attempt to get monitor data
    ZeroMemory( &monitorInfo, sizeof(monitorInfo) );
    monitorInfo.cbSize = sizeof(monitorInfo);
    if( GetMonitorInfo(defaultMonitor, &monitorInfo) == 0 )
    {
      return false;
    }
    if( width )
    {
      calculatedWidth  = abs(monitorInfo.rcMonitor.right
          - monitorInfo.rcMonitor.left);
      *width = calculatedWidth;
    }

    if( height )
    {
      calculatedHeight = abs(monitorInfo.rcMonitor.bottom
          - monitorInfo.rcMonitor.top);
      *height = calculatedHeight;
    }

    if( bpp )
    {
      ///TODO: Get Desktop BPP The Correct Way (TM)
      ///TODO: Distinguish between 15/16-BPP
      screenDC = GetDC(0);
      if( screenDC )
      {
        calculatedBPP = GetDeviceCaps(screenDC, PLANES)
            * GetDeviceCaps(screenDC, BITSPIXEL);
        *bpp = calculatedBPP;
        ReleaseDC(0, screenDC);
      }
    }

    return true;
  }

/*
 *  void CenterWindowToDesktop( HWND p_wnd )
 *
 *  Center specified window to Desktop.
 */
  void CenterWindowToDesktop( HWND p_wnd )
  {
    RECT windowRect;
    int windowWidth;
    int windowHeight;
    unsigned desktopWidth = 0;
    unsigned desktopHeight = 0;
    int centeredX;
    int centeredY;

    if( p_wnd )
    {
      GetDefaultDesktopResolution(
          &desktopWidth, &desktopHeight, NULL );

      GetWindowRect( p_wnd, &windowRect );
      windowWidth = (windowRect.right - windowRect.left);
      windowHeight = (windowRect.bottom - windowRect.top);

      centeredX = (desktopWidth - windowWidth) / 2;
      centeredY = (desktopHeight - windowHeight) / 2;

      SetWindowPos( p_wnd, 0, centeredX, centeredY,
        windowWidth, windowHeight,
        SWP_NOZORDER | SWP_NOACTIVATE );
    }
  }

/*
 *  =======================================
 *  Abstract system function implementation
 *  =======================================
 */

/*
 *  void sysFreeResources()
 *
 *  Implements Win32 system resource release function to release
 *    system resources allocated by sysw32.
 *
 */
  void sysFreeResources() {
    if( sysArgvw ) {
      LocalFree( sysArgvw );
      sysArgvw = NULL;
    }
  }

/*
 *  void sysExit( int exitCode )
 *
 *  Implements Win32 system exit function using best practices.
 */
#include <stdlib.h> // Temporary
  void sysExit( int exitCode ) {
    sysFreeResources();
    exit(exitCode); // Temporary
  }

/*
 *  LRESULT CALLBACK AppWndProc( HWND windowHandle, UINT message,
 *    WPARAM wParam, LPARAM lParam )
 *
 *  Win32 application window callback.
 */
  LRESULT CALLBACK sysw32WndProc( HWND windowHandle, UINT message,
      WPARAM wParam, LPARAM lParam )
  {
  	HDC paintDC;
    PAINTSTRUCT paintInfo;

    switch( message )
    {
	  case WM_PAINT:
	    paintDC = BeginPaint(windowHandle, &paintInfo);
	    EndPaint(windowHandle, &paintInfo);
	    return 0;

    case WM_ERASEBKGND:
      return 1;

    case WM_DISPLAYCHANGE:
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    default:
      return DefWindowProc(windowHandle, message, wParam, lParam);
    }

    return 0;
  }

/*
 *  void sysMessage( wchar_t* errorText )
 *
 *  Implements Win32 message pop-up
 */
  void sysMessage( wchar_t* messageText ) {
    MessageBoxW( sysw32AppWindow, messageText, L"Info", MB_OK );
  }

/*
 *  bool sysInitialize( wchar_t* title, unsigned width, unsigned height )
 *
 *  Implements Win32 app window creation.
 */

  bool sysInitialize( wchar_t* title, unsigned width, unsigned height ) {
    WNDCLASSEXW windowClassEx;
    RECT adjustedRect;
    DWORD windowStyle = WS_OVERLAPPEDWINDOW
        & (~(WS_THICKFRAME | WS_MAXIMIZEBOX));
    DWORD windowStyleEx = WS_EX_ACCEPTFILES;
    unsigned resultingWidth  = 0;
    unsigned resultingHeight = 0;
    unsigned desktopWidth = 0;
    unsigned desktopHeight = 0;

    if( sysw32AppWindow ) {
      return false;
    }

    // Determine resulting width/height from requested/desktop if 0
    GetDefaultDesktopResolution( &desktopWidth, &desktopHeight, NULL );

    resultingWidth = desktopWidth;
    if( width ) {
      resultingWidth = width;
    }

    resultingHeight = desktopHeight;
    if( height ) {
      resultingHeight = height;
    }

    // Register window class
    ZeroMemory( &windowClassEx, sizeof(windowClassEx) );
    windowClassEx.cbSize      = sizeof(windowClassEx);
    windowClassEx.style       = CS_HREDRAW | CS_VREDRAW;
    windowClassEx.lpfnWndProc = sysw32WndProc;
    windowClassEx.hInstance   = sysw32Instance;
    windowClassEx.hIcon       = LoadIcon(0, IDI_APPLICATION);
    windowClassEx.hCursor     = LoadCursor(0, IDC_ARROW);
    windowClassEx.lpszClassName = L"AppClass";
    windowClassEx.hIconSm     = LoadIcon(0, IDI_APPLICATION);
    if( RegisterClassExW(&windowClassEx) == 0 )
    {
      goto ReturnError;
    }

    // Create window
    SetRectEmpty( &adjustedRect );
    adjustedRect.right = resultingWidth;
    adjustedRect.bottom = resultingHeight;
    AdjustWindowRectEx( &adjustedRect, WS_OVERLAPPEDWINDOW,
        FALSE, 0 );

    sysw32AppWindow = CreateWindowExW(
      windowStyleEx,
      L"AppClass", title,
      windowStyle,
      CW_USEDEFAULT, CW_USEDEFAULT,
      (adjustedRect.right - adjustedRect.left),
      (adjustedRect.bottom - adjustedRect.top),
      NULL, NULL, sysw32Instance, NULL );
    if( sysw32AppWindow == NULL )
    {
      goto ReturnError;
    }

    CenterWindowToDesktop( sysw32AppWindow );

    ShowWindow( sysw32AppWindow, sysw32CmdShow );
    UpdateWindow( sysw32AppWindow );
    return true;

  ReturnError:
    return false;
  }

/*
 *  ==================================
 *  Win32 main function implementation
 *  ==================================
 */

/*
 *  Abstracted main function wrapper
 */
  extern int Main( int argc, wchar_t* argvw[] );

/*
 *  int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
 *    PWSTR pCmdLine, int nCmdShow )
 *
 *  Win32 main function to set up the abstracted program environment.
 */
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR pCmdLine, int nCmdShow ) {
  int retCode;

  sysw32Instance = hInstance;
  sysw32PrevInstance = hPrevInstance;
  sysw32CmdShow = nCmdShow;

  sysArgvw = CommandLineToArgvW(pCmdLine, &sysArgc);

  retCode = Main(sysArgc, sysArgvw);

  sysFreeResources();

  return retCode;
}
