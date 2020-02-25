
#define UNICODE
#define _UNICODE
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
 *  bool sysInitialize( wchar_t* title )
 *
 *  Implements Win32 app window creation.
 */

  bool sysInitialize( wchar_t* title ) {
    WNDCLASSEXW windowClassEx;
    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    DWORD windowStyleEx = WS_EX_ACCEPTFILES;

    if( sysw32AppWindow ) {
      return false;
    }

    // Register window class
    ZeroMemory( &windowClassEx, sizeof(windowClassEx) );
    windowClassEx.cbSize      = sizeof(windowClassEx);
    windowClassEx.style       = CS_HREDRAW | CS_VREDRAW;
    windowClassEx.lpfnWndProc = sysw32WndProc;
    windowClassEx.hInstance   = sysw32Instance;
    windowClassEx.hIcon       = LoadIcon(0, IDI_APPLICATION);
    windowClassEx.hCursor     = LoadCursor(0, IDC_ARROW);
    windowClassEx.lpszClassName = L"WBCWindow";
    windowClassEx.hIconSm     = LoadIcon(0, IDI_APPLICATION);
    if( RegisterClassExW(&windowClassEx) == 0 )
    {
      goto ReturnError;
    }

    // Create window
    sysw32AppWindow = CreateWindowExW(
      windowStyleEx,
      L"WBCWindow", title,
      windowStyle,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      NULL, NULL, sysw32Instance, NULL );
    if( sysw32AppWindow == NULL )
    {
      goto ReturnError;
    }

    ShowWindow( sysw32AppWindow, sysw32CmdShow );
    UpdateWindow( sysw32AppWindow );
    return true;

  ReturnError:
    return false;
  }

/*
 *  bool sysIsRunning()
 *
 *  Implements Win32 message routing on a per message basis,
 *    intended to be used as the main loop's condition test.
 */
  bool sysIsRunning() {
    MSG currentMessage;

    if( sysw32AppWindow )
    {
      ZeroMemory( &currentMessage, sizeof(currentMessage) );
      if( PeekMessage(&currentMessage, 0, 0, 0, PM_REMOVE) )
      {
        TranslateMessage( &currentMessage );
        DispatchMessage( &currentMessage );
      }

      return (currentMessage.message != WM_QUIT);
    }

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
