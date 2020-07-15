
#include "wbrushc.h"

#include "canvas.h"

int Main( int argc, wchar_t* argvw[] ) {
  if( appInitialize(L"Windbrush Concept") == false ) {
    appMessage( L"Inititalization error" );
    appExit(1);
  }

  if( !tabletInitialize() ) {
    appMessage( L"Error initializing tablet" );
  }

  Canvas canvas = CreateCanvas(100, 100);
  SurfaceInfo surfInfo = {};
  uint8_t* surface = (uint8_t*)SurfaceFromCanvas(canvas, &surfInfo);
  if( surface ) {
    memset( surface, 0, surfInfo.size );
  }

  while( appIsRunning() ) {
  }

  tabletFreeResources();
  appFreeResources();

  return 0;
}
