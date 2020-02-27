#ifndef H_APP
#define H_APP

/*
 *  Application definitions
 */
  void appMessage( wchar_t* messageText );

  void appFreeResources();
  void appExit( int exitCode );
  bool appInitialize();

  bool appIsRunning();

#endif
