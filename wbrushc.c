
#include "wbrushc.h"

int Main( int argc, wchar_t* argvw[] ) {
  if( appInitialize(L"Windbrush Concept") == false ) {
    appMessage( L"Inititalization error" );
    appExit(1);
  }

  while( appIsRunning() ) {
  }

  return 0;
}
