
#define UNICODE
#define _UNICODE
#include "windows.h"

#include <stdlib.h>

#include "canvas.h"
#include "sysw32.h"

  typedef struct Win32Canvas {
    HANDLE appWindow;
    DIBSECTION dibSect;
  } Win32Canvas;

  Canvas CreateCanvas( unsigned width, unsigned height ) {
    Win32Canvas* newCanvas = NULL;

    newCanvas = calloc(1, sizeof(Win32Canvas));
    if( newCanvas == NULL ) {
      goto OnError;
    }

    newCanvas->appWindow = AppWindowHandle();

    ///TODO: Set up buffer

    return newCanvas;

  OnError:
    return NULL;
  }

  Canvas ResizeCanvas( Canvas canvas, unsigned width, unsigned height ) {
    return NULL;
  }

  void FreeCanvas( Canvas* canvasPtr ) {
    Canvas canvas;

    if( canvasPtr ) {
      canvas = *canvasPtr;

      if( canvas ) {
        ///TODO: Release dibSect/etc

        free( *canvasPtr );
        *canvasPtr = NULL;
      }
    }
  }

  void* SurfaceFromCanvas( Canvas source, SurfaceInfo* info ) {
    return NULL;
  }
