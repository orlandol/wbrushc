
#include <stddef.h>

#include "sysw32.h"

  int argc;
  wchar_t** argvw;

  extern int WMain();

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR pCmdLine, int nCmdShow ) {

  return WMain();
}
