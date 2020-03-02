
#define UNICODE
#define _UNICODE
#include "windows.h"

#include <stdbool.h>
#include <string.h>

#include "WINTAB.H"
 #define PACKETDATA (PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE)
 #define PACKETMODE PK_BUTTONS
#include "PKTDEF.H"

/*
 *  Internal tablet declarations
 */

  // Begin - Declarations from Utils.h by Wacom Corporation
  typedef UINT ( API * WTINFOA ) ( UINT, UINT, LPVOID );
  typedef HCTX ( API * WTOPENA )( HWND, LPLOGCONTEXTA, BOOL );
  typedef BOOL ( API * WTGETA ) ( HCTX, LPLOGCONTEXT );
  typedef BOOL ( API * WTSETA ) ( HCTX, LPLOGCONTEXT );

  typedef UINT ( API * WTINFOW ) ( UINT, UINT, LPVOID );
  typedef HCTX ( API * WTOPENW )( HWND, LPLOGCONTEXTW, BOOL );
  typedef BOOL ( API * WTGETW ) ( HCTX, LPLOGCONTEXT );
  typedef BOOL ( API * WTSETW ) ( HCTX, LPLOGCONTEXT );

  typedef BOOL ( API * WTCLOSE ) ( HCTX );
  typedef BOOL ( API * WTENABLE ) ( HCTX, BOOL );
  typedef BOOL ( API * WTPACKET ) ( HCTX, UINT, LPVOID );
  typedef BOOL ( API * WTOVERLAP ) ( HCTX, BOOL );
  typedef BOOL ( API * WTSAVE ) ( HCTX, LPVOID );
  typedef BOOL ( API * WTCONFIG ) ( HCTX, HWND );
  typedef HCTX ( API * WTRESTORE ) ( HWND, LPVOID, BOOL );
  typedef BOOL ( API * WTEXTSET ) ( HCTX, UINT, LPVOID );
  typedef BOOL ( API * WTEXTGET ) ( HCTX, UINT, LPVOID );
  typedef BOOL ( API * WTQUEUESIZESET ) ( HCTX, int );
  typedef int  ( API * WTDATAPEEK ) ( HCTX, UINT, UINT, int, LPVOID, LPINT);
  typedef int  ( API * WTPACKETSGET ) (HCTX, int, LPVOID);
  // End - Declarations from Utils.h by Wacom Corporation

  HINSTANCE wintabDll = NULL;

  WTINFOA dllWTInfoA = NULL;
  WTOPENA dllWTOpenA = NULL;
  WTGETA dllWTGetA = NULL;
  WTSETA dllWTSetA = NULL;

  WTINFOW dllWTInfoW = NULL;
  WTOPENW dllWTOpenW = NULL;
  WTGETW dllWTGetW = NULL;
  WTSETW dllWTSetW = NULL;

  WTCLOSE dllWTClose = NULL;
  WTENABLE dllWTEnable = NULL;
  WTPACKET dllWTPacket = NULL;
  WTOVERLAP dllWTOverlap = NULL;
  WTSAVE dllWTSave = NULL;
  WTCONFIG dllWTConfig = NULL;
  WTRESTORE dllWTRestore = NULL;
  WTEXTSET dllWTExtSet = NULL;
  WTEXTGET dllWTExtGet = NULL;

  bool wintabInitialize();
  void wintabFreeResources();

/*
 *  bool tabletInitialize()
 *
 *  Initializes the first tablet API available.
 */
  bool wintabInitialize() {
    wchar_t wacomName[50] = {};

    if( wintabDll ) {
      return true;
    }

    wintabDll = LoadLibraryW(L"Wintab32.dll");
    if( wintabDll == NULL ) {
      return false;
    }

    dllWTInfoA = (WTINFOA)GetProcAddress(wintabDll, "WTInfoA");
    dllWTOpenA = (WTINFOA)GetProcAddress(wintabDll, "WTOpenA");
    dllWTGetA = (WTINFOA)GetProcAddress(wintabDll, "WTGetA");
    dllWTSetA = (WTINFOA)GetProcAddress(wintabDll, "WTSetA");
    dllWTInfoW = (WTINFOA)GetProcAddress(wintabDll, "WTInfoW");
    dllWTOpenW = (WTINFOA)GetProcAddress(wintabDll, "WTOpenW");
    dllWTGetW = (WTINFOA)GetProcAddress(wintabDll, "WTGetW");
    dllWTSetW = (WTINFOA)GetProcAddress(wintabDll, "WTSetW");
    dllWTClose = (WTINFOA)GetProcAddress(wintabDll, "WTClose");
    dllWTEnable = (WTINFOA)GetProcAddress(wintabDll, "WTEnable");
    dllWTPacket = (WTINFOA)GetProcAddress(wintabDll, "WTPacket");
    dllWTOverlap = (WTINFOA)GetProcAddress(wintabDll, "WTOverlap");
    dllWTSave = (WTINFOA)GetProcAddress(wintabDll, "WTSave");
    dllWTConfig = (WTINFOA)GetProcAddress(wintabDll, "WTConfig");
    dllWTRestore = (WTINFOA)GetProcAddress(wintabDll, "WTRestore");
    dllWTExtSet = (WTINFOA)GetProcAddress(wintabDll, "WTExtSet");
    dllWTExtGet = (WTINFOA)GetProcAddress(wintabDll, "WTExtGet");

    if( !(dllWTInfoA && dllWTOpenA && dllWTGetA && dllWTSetA &&
        dllWTInfoW && dllWTOpenW && dllWTGetW && dllWTSetW &&
        dllWTClose && dllWTEnable && dllWTPacket && dllWTOverlap &&
        dllWTSave && dllWTConfig && dllWTRestore &&
        dllWTExtSet && dllWTExtGet) ) {
      goto ReturnError;
    }

    if( dllWTInfoW(0, 0, NULL) == FALSE ) {
      goto ReturnError;
    }

    dllWTInfoW( WTI_DEVICES, DVC_NAME, wacomName );
    if( wcsncmp(wacomName, L"WACOM", 5) != 0 ) {
      goto ReturnError;
    }

    return true;

  ReturnError:
    wintabFreeResources();
    return false;
  }

  void wintabFreeResources() {
    if( wintabDll ) {
      dllWTInfoA = NULL;
      dllWTOpenA = NULL;
      dllWTGetA = NULL;
      dllWTSetA = NULL;
      dllWTInfoW = NULL;
      dllWTOpenW = NULL;
      dllWTGetW = NULL;
      dllWTSetW = NULL;
      dllWTClose = NULL;
      dllWTEnable = NULL;
      dllWTPacket = NULL;
      dllWTOverlap = NULL;
      dllWTSave = NULL;
      dllWTConfig = NULL;
      dllWTRestore = NULL;
      dllWTExtSet = NULL;
      dllWTExtGet = NULL;
      dllWTInfoA = NULL;
      dllWTOpenA = NULL;
      dllWTGetA = NULL;
      dllWTSetA = NULL;
      dllWTInfoW = NULL;
      dllWTOpenW = NULL;
      dllWTGetW = NULL;
      dllWTSetW = NULL;
      dllWTClose = NULL;
      dllWTEnable = NULL;
      dllWTPacket = NULL;
      dllWTOverlap = NULL;
      dllWTSave = NULL;
      dllWTConfig = NULL;
      dllWTRestore = NULL;
      dllWTExtSet = NULL;
      dllWTExtGet = NULL;

      FreeLibrary( wintabDll );
      wintabDll = NULL;
    }
  }

  bool tabletInitialize() {
    ///TODO: Initialize Microsoft Tablet Input

    // Attempt to initialize the Wacom tablet API
    if( wintabInitialize() ) {
      return true;
    }

    return false;
  }

  void tabletFreeResources() {
    wintabFreeResources();
  }
