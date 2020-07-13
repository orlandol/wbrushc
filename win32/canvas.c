
#define UNICODE
#define _UNICODE
#include "windows.h"

#include <stdlib.h>
#include <stdio.h>

#include "canvas.h"
#include "sysw32.h"

  typedef struct BITMAPINFOFULL {
    BITMAPINFOHEADER bmiHeader;
    union {
      DWORD bmiBitFields[3];
      RGBQUAD bmiColors[256];
    };
  } BITMAPINFOFULL;

  typedef struct BITMAPINFORGB {
    BITMAPINFOHEADER bmiHeader;
    DWORD bmiBitFields[3];
  } BITMAPINFORGB;

  typedef struct Win32Canvas {
    HWND appWindow;
    HBITMAP dibSect;
    BITMAPINFORGB dibInfo;
    VOID* dibBits;
    UINT dibUsage;
    PixelFormat pixelFormat;
  } Win32Canvas;

  Canvas CreateCanvas( unsigned width, unsigned height ) {
    Win32Canvas* newCanvas = NULL;
    HBITMAP dibSect = NULL;
    BITMAPINFOFULL* dibInfo = NULL;
    PixelFormat pixelFormat;
    HWND appWindow = NULL;
    HDC winDC = NULL;
    UINT dibUsage;
    DWORD lineSize;
    int bitsPixel;
    int planes;
    unsigned winBPP;

    if( !(width && height) ) {
      goto OnError;
    }

    appWindow = AppWindowHandle();
    if( appWindow == NULL ) {
      goto OnError;
    }

    newCanvas = calloc(1, sizeof(Win32Canvas));
    if( newCanvas == NULL ) {
      goto OnError;
    }

    newCanvas->appWindow = appWindow;

    winDC = GetDC(appWindow);
    bitsPixel = GetDeviceCaps(winDC, BITSPIXEL);
    planes = GetDeviceCaps(winDC, PLANES);
    winBPP = bitsPixel * planes;

    dibInfo = &(newCanvas->dibInfo);
    dibInfo->bmiHeader.biSize = sizeof(BITMAPINFO);
    dibInfo->bmiHeader.biWidth = width;
    dibInfo->bmiHeader.biHeight = -height; // Requires BI_RGB/BITFIELDS
    dibInfo->bmiHeader.biPlanes = 1; // Must be 1
    dibInfo->bmiHeader.biBitCount = winBPP;
    dibInfo->bmiHeader.biCompression = BI_RGB; // Default to BI_RGB

    lineSize = (((width * winBPP) + 31) & (~31)) / 8;
    dibInfo->bmiHeader.biSizeImage = lineSize * height;

    dibUsage = DIB_RGB_COLORS;
    switch( winBPP ) {
    case 8:
      ///TODO: Set up an 8-BPP DIB Section
      //dibUsage = DIB_PAL_COLORS;
      //dibInfo->bmiHeader.biClrUsed = 256; //?
      //break;
      goto OnError; // Currently unsupported

    case 15:
    case 16:
      ///TODO: Set up bmiColors[0..2]? as BI_BITFIELDS?
      //dibInfo->bmiHeader.biClrUsed = (1 << winBPP); //?
      //break;
      goto OnError; // Currently unsupported

    case 24:
      break;

    case 32:
      ///TODO: Set up bmiColors[0..2]? as BI_BITFIELDS?
      break;

    default:
      goto OnError;
    }
    newCanvas->dibUsage = dibUsage;

    ///TODO: Perform sanity check on dibInfo fields

    dibSect = CreateDIBSection( winDC, &(newCanvas->dibInfo),
      dibUsage,
      &(newCanvas->dibBits), // Pointer to pixels; GdiFlush before write
      NULL, 0 // No file mapping specified; No offset needed
    );
    newCanvas->dibSect = dibSect;

    ReleaseDC( appWindow, winDC );
    winDC = NULL;

    return newCanvas;

  OnError:
    if( dibSect ) {
      DeleteObject( dibSect );
      dibSect = NULL;
    }

    if( winDC ) {
      ReleaseDC( appWindow, winDC );
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
    Win32Canvas* canvas;

    if( canvasPtr ) {
      canvas = *canvasPtr;

      if( canvas ) {
        if( canvas->dibSect ) {
          DeleteObject( canvas->dibSect );
          canvas->dibSect = NULL;
        }

        free( *canvasPtr );
        *canvasPtr = NULL;
      }
    }
  }

  void* SurfaceFromCanvas( Canvas source, SurfaceInfo* info ) {
    Win32Canvas* w32Canvas = (Win32Canvas*)source;
    SurfaceInfo translatedInfo = {};

    if( w32Canvas && info ) {
      if( w32Canvas->dibBits ) {
        translatedInfo.width = w32Canvas->dibInfo.bmiHeader.biWidth;
        translatedInfo.height = abs(w32Canvas->dibInfo.bmiHeader.biHeight);
        translatedInfo.bpp = w32Canvas->dibInfo.bmiHeader.biBitCount;
        translatedInfo.size = w32Canvas->dibInfo.bmiHeader.biSizeImage;
        translatedInfo.pixelFormat = w32Canvas->pixelFormat;

        *info = translatedInfo;
        return w32Canvas->dibBits;
      }
    }

    return NULL;
  }
