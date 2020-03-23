
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

  bool CalcPixelFormat16( HDC drawDC, PixelFormat* pixelFormat ) {
    HDC testDC = NULL;
    HBITMAP testHandle = NULL;

    return false;

  OnError:
    return false;
  }

  bool CalcPixelFormat24( HDC drawDC, PixelFormat* pixelFormat ) {
    return false;
  }

  bool CalcPixelFormat32( HDC drawDC, PixelFormat* pixelFormat ) {
    return false;
  }

  Canvas CreateCanvas( unsigned width, unsigned height ) {
    Win32Canvas* newCanvas = NULL;
    PixelFormat pixelFormat;
    PAINTSTRUCT tempPaint;
    HDC winDC = NULL;
    int bitsPixel;
    int planes;
    unsigned winBPP;
    bool result;

    newCanvas = calloc(1, sizeof(Win32Canvas));
    if( newCanvas == NULL ) {
      goto OnError;
    }

    newCanvas->appWindow = AppWindowHandle();

    winDC = BeginPaint(newCanvas->appWindow, &tempPaint);
    bitsPixel = GetDeviceCaps(winDC, BITSPIXEL);
    planes = GetDeviceCaps(winDC, PLANES);
    winBPP = bitsPixel * planes;

    result = false;
    switch( winBPP ) {
    case 8:
      break;

    case 15:
    case 16:
      result = CalcPixelFormat16(winDC, &pixelFormat);
      break;

    case 24:
      result = CalcPixelFormat24(winDC, &pixelFormat);
      break;

    case 32:
      result = CalcPixelFormat32(winDC, &pixelFormat);
      break;
    }

    if( result == false ) {
      goto OnError;
    }

    EndPaint( winDC, &tempPaint );
    winDC = NULL;

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
