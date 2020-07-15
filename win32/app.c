
#define UNICODE
#define _UNICODE
#include "windows.h"
#include "shellapi.h"

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "app.h"
#include "sysw32.h"

/*
 *  Internal app declarations
 */
  HINSTANCE appInstance = NULL;
  HINSTANCE appPrevInstance = NULL;
  int appCmdShow = SW_SHOWNORMAL;

  HWND appWindow = NULL;

  int appArgc = 0;
  wchar_t** appArgvw = NULL;

  HANDLE AppWindowHandle() {
    return appWindow;
  }

/*
 *  LRESULT CALLBACK AppWndProc( HWND windowHandle, UINT message,
 *    WPARAM wParam, LPARAM lParam )
 *
 *  Win32 app window callback.
 */
  LRESULT CALLBACK appWndProc( HWND windowHandle, UINT message,
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
 *  void appFreeResources()
 *
 *  Release system resources allocated by app.
 *
 */
  void appFreeResources() {
    if( appWindow ) {
      DestroyWindow( appWindow ); // Temporary
      appWindow = NULL;
    }

    if( appArgvw ) {
      LocalFree( appArgvw );
      appArgvw = NULL;
    }
  }

/*
 *  void appExit( int exitCode )
 *
 *  Exit app function using best practices.
 */
#include <stdlib.h> // Temporary
  void appExit( int exitCode ) {
    appFreeResources();
    exit(exitCode); // Temporary
  }

/*
 *  void appMessage( wchar_t* errorText )
 *
 *  Abstracted Win32 message pop-up
 */
  void appMessage( wchar_t* messageText ) {
    MessageBoxW( appWindow, messageText, L"Info", MB_OK );
  }

/*
 *  bool appInitialize( wchar_t* title )
 *
 *  Abstracted Win32 app window creation.
 */

  bool appInitialize( wchar_t* title ) {
    WNDCLASSEXW windowClassEx;
    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    DWORD windowStyleEx = WS_EX_ACCEPTFILES;

    if( appWindow ) {
      return false;
    }

    // Register window class
    ZeroMemory( &windowClassEx, sizeof(windowClassEx) );
    windowClassEx.cbSize      = sizeof(windowClassEx);
    windowClassEx.style       = CS_HREDRAW | CS_VREDRAW;
    windowClassEx.lpfnWndProc = appWndProc;
    windowClassEx.hInstance   = appInstance;
    windowClassEx.hIcon       = LoadIcon(0, IDI_APPLICATION);
    windowClassEx.hCursor     = LoadCursor(0, IDC_ARROW);
    windowClassEx.lpszClassName = L"WBCWindow";
    windowClassEx.hIconSm     = LoadIcon(0, IDI_APPLICATION);
    if( RegisterClassExW(&windowClassEx) == 0 )
    {
      goto ReturnError;
    }

    // Create window
    appWindow = CreateWindowExW(
      windowStyleEx,
      L"WBCWindow", title,
      windowStyle,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      NULL, NULL, appInstance, NULL );
    if( appWindow == NULL )
    {
      goto ReturnError;
    }

    ShowWindow( appWindow, appCmdShow );
    UpdateWindow( appWindow );
    return true;

  ReturnError:
    return false;
  }

/*
 *  bool appIsRunning()
 *
 *  Abstracted single Win32 message routing function for the
 *    main loop's condition test.
 */
  bool appIsRunning() {
    MSG currentMessage;

    if( appWindow )
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
 *  void appDrawCanvas( Canvas canvas )
 *
 *  Draws the contents of canvas to the active display.
 *    Canvas is drawn starting from the upper-left corner
 *    of the work area, whose excess is clipped to the
 *    lower-right corner.
 */
  void appDrawCanvas( Canvas canvas ) {
  }

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

  appInstance = hInstance;
  appPrevInstance = hPrevInstance;
  appCmdShow = nCmdShow;

  appArgvw = CommandLineToArgvW(pCmdLine, &appArgc);

  retCode = Main(appArgc, appArgvw);

  appFreeResources();

  return retCode;
}
