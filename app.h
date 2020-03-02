#ifndef H_APP
#define H_APP

/*
 *  Application definitions
 */
  void appMessage( wchar_t* messageText );

  bool appInitialize( wchar_t* title );
  void appFreeResources();

  void appExit( int exitCode );

  bool appIsRunning();

#endif
