
#include "wbrushc.h"

int Main( int argc, wchar_t* argvw[] ) {
  if( sysInitialize(L"Windbrush", 0, 0) == false ) {
    sysMessage( L"Error instantiating Windbrush" );
    sysExit(1);
  }

  return 0;
}
