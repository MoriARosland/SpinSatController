#include <stdio.h>
#include "raylib/raylib.h"
#include "raylib/clay_renderer_raylib.c"
#include "error_handler.h"

#define CLAY_IMPLEMENTATION
#include "clay.h"

const float screenWidth = 1280.0f;
const float screenHeight = 768.0f;

/// @brief Creates the layout for the UI within a Clay_BeginLayout/Clay_EndLayout boundry.
/// @return Clay_RenderCommandArray
Clay_RenderCommandArray CreateLayout(void)
{
    Clay_BeginLayout();

    CLAY({.id = CLAY_ID("OuterContainer"), .layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0)}, .padding = {16, 16, 16, 16}, .childGap = 16}, .backgroundColor = {245, 245, 245, 255}})
    {
        CLAY({.id = CLAY_ID("TopHeaderBar"), .layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(64)}, .padding = {16, 16, 16, 16}, .childGap = 16}, .backgroundColor = {200, 200, 200, 255}, .cornerRadius = {16, 16, 16, 16}}) {}
        CLAY({.id = CLAY_ID("MainBody"), .layout = {.layoutDirection = CLAY_LEFT_TO_RIGHT, .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0)}, .padding = {16, 16, 16, 16}, .childGap = 16}, .backgroundColor = {150, 150, 255, 255}}) {}
    }

    return Clay_EndLayout();
}

void UpdateDrawFrame(Font *fonts)
{
    // Add logic for handling events here

    // Build the layout
    Clay_RenderCommandArray renderCommands = CreateLayout();

    // Render the layout
    BeginDrawing();
    ClearBackground(RAYWHITE);
    Clay_Raylib_Render(renderCommands, fonts);
    EndDrawing();
}

int main(void)
{

    uint64_t clayMemorySize = Clay_MinMemorySize();
    Clay_Arena memoryArena = Clay_CreateArenaWithCapacityAndMemory(clayMemorySize, malloc(clayMemorySize));
    Clay_Dimensions dimensions = {
        .width = screenWidth,
        .height = screenHeight,
    };

    Clay_Initialize(memoryArena, dimensions, (Clay_ErrorHandler){HandleClayErrors});
    Clay_Raylib_Initialize((int)screenWidth, (int)screenHeight, "SpinSat Controller", 0);

    Font fonts[1];
    fonts[0] = GetFontDefault();
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

    while (!WindowShouldClose())
    {
        Clay_RenderCommandArray renderCommands = CreateLayout();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        Clay_Raylib_Render(renderCommands, fonts);
        EndDrawing();
    }

    Clay_Raylib_Close();
    return 0;
}