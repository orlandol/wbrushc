
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

  bool CalcPixelFormat16( HWND winHandle, PixelFormat* pixelFormat ) {
    return false;
  }

  bool CalcPixelFormat24( HWND winHandle, PixelFormat* pixelFormat ) {
    return false;
  }

  bool CalcPixelFormat32( HWND winHandle, PixelFormat* pixelFormat ) {
    HDC winDC = NULL;
    HBITMAP winBitmap = NULL;
    BITMAP bmpInfo = {};
    bool result = false;
    wchar_t msgBuffer[2048] = {};

    winDC = GetDC(winHandle);
    if( winDC == NULL ) {
      goto OnError;
    }

    winBitmap = CreateCompatibleBitmap(winDC, 1, 1);
    if( winBitmap == NULL ) {
      goto OnError;
    }

    GetObject( winBitmap, sizeof(BITMAP), &bmpInfo );

    snwprintf( msgBuffer, sizeof(msgBuffer) - 1,
      L"      bmType: %i\n"
      L"     bmWidth: %i\n"
      L"    bmHeight: %i\n"
      L"bmWidthBytes: %i\n"
      L"    bmPlanes: %i\n"
      L" bmBitsPixel: %i\n"
      L"      bmBits: %p\n",
      bmpInfo.bmType,
      bmpInfo.bmWidth,
      bmpInfo.bmHeight,
      bmpInfo.bmWidthBytes,
      bmpInfo.bmPlanes,
      bmpInfo.bmBitsPixel,
      bmpInfo.bmBits
    );

    MessageBox( AppWindowHandle(), msgBuffer, L"CalcPixelFormat32", MB_OK );

    result = true;

  OnError:
    if( winBitmap ) {
      DeleteObject( winBitmap );
    }

    if( winDC ) {
      ReleaseDC( winHandle, winDC );
      winDC = NULL;
    }

    return result;
  }

  typedef struct BITMAP256 {
    BITMAP bmp;
    
  } BITMAP256;

  Canvas CreateCanvas( unsigned width, unsigned height ) {
    Win32Canvas* newCanvas = NULL;
    PixelFormat pixelFormat;
    PAINTSTRUCT tempPaint;
    HWND appWindow = NULL;
    HDC winDC = NULL;
    int bitsPixel;
    int planes;
    unsigned winBPP;
    bool result;

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

    result = false;
    switch( winBPP ) {
    case 8:
      break;

    case 15:
    case 16:
      result = CalcPixelFormat16(appWindow, &pixelFormat);
      break;

    case 24:
      result = CalcPixelFormat24(appWindow, &pixelFormat);
      break;

    case 32:
      result = CalcPixelFormat32(appWindow, &pixelFormat);
      break;
    }

    if( result == false ) {
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
