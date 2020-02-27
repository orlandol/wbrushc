
#define UNICODE
#define _UNICODE
#include "windows.h"

#include "canvas.h"

  typedef struct Win32Canvas {
    DIBSECTION dibSect;
  } Win32Canvas;

  Canvas CreateCanvas( unsigned width, unsigned height, unsigned bpp ) {
    return NULL;
  }

  void FreeCanvas( Canvas* canvas ) {
  }


  void* SurfaceFromCanvas( Canvas source, SurfaceInfo* info ) {
    return NULL;
  }
