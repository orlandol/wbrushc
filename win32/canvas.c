
#define UNICODE
#define _UNICODE
#include "windows.h"

#include <stdlib.h>
#include <stdio.h>

#include "canvas.h"
#include "sysw32.h"

  typedef struct Win32Canvas {
    HWND appWindow;
    DIBSECTION dibSect;
  } Win32Canvas;

  Canvas CreateCanvas( unsigned width, unsigned height ) {
    Win32Canvas* newCanvas = NULL;
    PixelFormat pixelFormat;
    PAINTSTRUCT tempPaint;
    HWND appWindow = NULL;
    HDC winDC = NULL;
    int bitsPixel;
    int planes;
    unsigned winBPP;

    newCanvas = calloc(1, sizeof(Win32Canvas));
    if( newCanvas == NULL ) {
      goto OnError;
    }

    appWindow = AppWindowHandle();
    newCanvas->appWindow = appWindow;

    winDC = BeginPaint(appWindow, &tempPaint);
    bitsPixel = GetDeviceCaps(winDC, BITSPIXEL);
    planes = GetDeviceCaps(winDC, PLANES);
    winBPP = bitsPixel * planes;
    EndPaint( winDC, &tempPaint );
    winDC = NULL;

    switch( winBPP ) {
    case 8:
      break;

    case 15:
    case 16:
      break;

    case 24:
      break;

    case 32:
      break;

    default:
      goto OnError;
    }

    ///TODO: Set up buffer

    return newCanvas;

  OnError:
    if( winDC ) {
      EndPaint( winDC, &tempPaint );
      winDC = NULL;
    }

    if( newCanvas ) {
      free( newCanvas );
      newCanvas = NULL;
    }

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
