#ifndef H_WBRUSHC
#define H_WBRUSHC

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/*
 * Abstracted system function prototypes
 */
  extern void sysFreeResources();
  extern void sysExit( int exitCode );
  extern bool sysInitialize( wchar_t* title, unsigned width, unsigned height );

#endif
