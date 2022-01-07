#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define APPVERSTR "Windbrush Concept v0.1 Alpha"

static const int defaultAppWidth = 800;
static const int defaultAppHeight = 500;

int main( int argc, char** argv ) {

    SetConfigFlags( FLAG_WINDOW_RESIZABLE );
    InitWindow( defaultAppWidth, defaultAppHeight, APPVERSTR );

    SetExitKey( 0 ); // KEY_NONE?

    // TODO: Load resources / Initialize variables at this point

    RenderTexture2D target = LoadRenderTexture(defaultAppWidth, defaultAppHeight);

    Shader shader = LoadShader(0, TextFormat("shaders/#1/wbray.fs", GLSL_VERSION));

    SetTargetFPS(60);

    while( !WindowShouldClose() ) {
        // Update [state]

        // Draw
        BeginTextureMode( target );
            ClearBackground( BLACK );
        EndTextureMode();

        BeginDrawing();

            ClearBackground(BLACK);

            // TODO: Draw everything that requires to be drawn at this point:

            BeginShaderMode( shader );
                DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2){ 0.0f, 0.0f }, WHITE);
            EndShaderMode();
        EndDrawing();
    }

    // De-Initialization

    // TODO: Unload all loaded resources at this point

    CloseWindow();

    return 0;
}
