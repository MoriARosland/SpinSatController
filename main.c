#include <stdio.h>
#include "raylib/raylib.h"
#include "raylib/clay_renderer_raylib.c"

#define CLAY_IMPLEMENTATION
#include "clay.h"

const float screenWidth = 1024.0f;
const float screenHeight = 768.0f;

void HandleClayErrors(Clay_ErrorData errorData)
{
    // See the Clay_ErrorData struct for more information
    printf("%s", errorData.errorText.chars);
    switch (errorData.errorType)
    {
        // etc
    }
}

int main(void)
{
    printf("Booting up...\n");

    Clay_Raylib_Initialize((int)screenWidth, (int)screenHeight, "SpinSat Controller", 0);

    uint64_t clayMemorySize = Clay_MinMemorySize();
    Clay_Arena memoryArena = Clay_CreateArenaWithCapacityAndMemory(clayMemorySize, malloc(clayMemorySize));
    Clay_Dimensions dimensions = {
        .width = screenWidth,
        .height = screenHeight,
    };

    Clay_Initialize(memoryArena, dimensions, (Clay_ErrorHandler){HandleClayErrors});

    while (!WindowShouldClose())
    {
        /* code */

        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    return 0;
}