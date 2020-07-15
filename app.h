#ifndef H_APP
#define H_APP

#include "canvas.h"

/*
 *  Application definitions
 */
  void appMessage( wchar_t* messageText );

  bool appInitialize( wchar_t* title );
  void appFreeResources();

  void appExit( int exitCode );

  bool appIsRunning();

  void appDrawCanvas( Canvas canvas );

#endif
