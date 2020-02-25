
#include "wbrushc.h"

int Main( int argc, wchar_t* argvw[] ) {
  if( sysInitialize(L"Windbrush Concept") == false ) {
    sysMessage( L"Inititalization error" );
    sysExit(1);
  }

  while( sysIsRunning() ) {
  }

  return 0;
}
