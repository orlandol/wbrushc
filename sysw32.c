
#include "windows.h"
#include "shellapi.h"

#include "sysw32.h"

/*
 * Win32 implementation declarations
 */
  HINSTANCE sysInstance = NULL;
  HINSTANCE sysPrevInstance = NULL;
  int sysCmdShow = SW_SHOWNORMAL;

  int sysArgc = 0;
  wchar_t** sysArgvw = NULL;

/*
 * Abstracted main function wrapper
 */
  extern int Main( int argc, wchar_t* argvw[] );

/*
 * void sysFreeResources()
 *
 * Implements Win32 system resource release function to release
 *   system resources allocated by sysw32.
 *
 */
  void sysFreeResources() {
    if( sysArgvw ) {
      LocalFree( sysArgvw );
      sysArgvw = NULL;
    }
  }

/*
 * void sysExit( int exitCode )
 *
 * Implements Win32 system exit function using best practices
 *   to properly exit the program.
 */
#include <stdlib.h> // Temporary
  void sysExit( int exitCode ) {
    sysFreeResources();
    exit(exitCode); // Temporary
  }

/*
 * bool sysInitialize( wchar_t* title, unsigned width, unsigned height )
 *
 * Implements Win32 app window creation.
 */
  bool sysInitialize( wchar_t* title, unsigned width, unsigned height ) {
    return false;
  }

/*
 * int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
 *   PWSTR pCmdLine, int nCmdShow )
 *
 * Win32 main function to set up the abstracted program environment.
 */
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR pCmdLine, int nCmdShow ) {
  int retCode;

  sysArgvw = CommandLineToArgvW(pCmdLine, &sysArgc);

  retCode = Main(sysArgc, sysArgvw);

  sysFreeResources();

  return retCode;
}
