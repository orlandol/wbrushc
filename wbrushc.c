
#include "tigr/tigr.h"
#include "tigr/tigr.c"

#define EASYTAB_IMPLEMENTATION
#include "easytab\easytab.h"

  typedef struct Theme {
    TPixel winBack;
  } Theme;

Theme defaultTheme = {};
Theme* activeTheme = &defaultTheme;

int main(int argc, char *argv[])
{
	Tigr *screen = tigrWindow(640, 400, "Windbrush Concept", TIGR_AUTO);

    if( EasyTab_Load(screen->handle) != EASYTAB_OK ) {
    }

    defaultTheme.winBack = tigrRGB(0x80, 0x80, 0x80);

	while( !tigrClosed(screen) )
	{
		tigrClear( screen, activeTheme->winBack );
		tigrUpdate( screen );
	}

	tigrFree( screen );

    EasyTab_Unload();

	return 0;
}
