#ifndef H_CANVAS
#define H_CANVAS

#include <stdbool.h>

/*
 *  Abstract canvas definitions
 */
  typedef void* CanvasImpl;
  typedef CanvasImpl Canvas;

  Canvas CreateCanvas( unsigned width, unsigned height, unsigned bpp );
  void FreeCanvas( Canvas* canvas );

/*
 *  Abstract surface definitions
 */
  typedef struct SurfaceInfo {
    unsigned width;
    unsigned height;
    unsigned bpp;
    size_t size;
    unsigned rShift;
    unsigned gShift;
    unsigned bShift;
    unsigned aShift;
    unsigned rMask;
    unsigned gMask;
    unsigned bMask;
    unsigned aMash;
  } SurfaceInfo;

  void* SurfaceFromCanvas( Canvas source, SurfaceInfo* info );

#endif
