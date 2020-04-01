
#include "wbrushc.h"

int Main( int argc, wchar_t* argvw[] ) {
  if( appInitialize(L"Windbrush Concept") == false ) {
    appMessage( L"Inititalization error" );
    appExit(1);
  }

  if( !tabletInitialize() ) {
    appMessage( L"Error initializing tablet" );
  }

  Canvas canvas = CreateCanvas(100, 100);

  while( appIsRunning() ) {
  }

  tabletFreeResources();
  appFreeResources();

  return 0;
}
