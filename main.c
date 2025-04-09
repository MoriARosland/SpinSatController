#include "ble_handler.h"
#include "error_handler.h"
#include "raylib/clay_renderer_raylib.c"
#include "raylib/raylib.h"
#include <stdio.h>

#define CLAY_IMPLEMENTATION
#include "clay.h"

#define COLOR_WHITE {245, 245, 245, 255}
#define COLOR_GRAY {200, 200, 200, 255}
#define COLOR_BLUE {150, 150, 255, 255}

const float screenWidth = 1280.0f;
const float screenHeight = 768.0f;

Clay_TextElementConfig textConfig = {
    .fontId = 0, .fontSize = 24, .textColor = {0, 0, 0, 255}};

Clay_LayoutConfig defaultLayoutStyle = (Clay_LayoutConfig){
    .layoutDirection = CLAY_TOP_TO_BOTTOM,
    .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0)},
    .padding = {16, 16, 16, 16},
    .childGap = 16,
};

/// @brief Creates a Clay_LayoutConfig with a configurable height
/// @param height The desired height for the layout
/// @return Clay_LayoutConfig with the specified height
Clay_LayoutConfig CreateLayoutConfigWithHeight(float height) {
    return (Clay_LayoutConfig){
        .layoutDirection = CLAY_TOP_TO_BOTTOM,
        .sizing = {.width = CLAY_SIZING_GROW(0),
                   .height = CLAY_SIZING_FIXED(height)},
        .padding = {16, 16, 16, 16},
        .childGap = 16,
    };
}

/// @brief Creates the layout for the UI within a
/// Clay_BeginLayout/Clay_EndLayout boundry.
/// @return Clay_RenderCommandArray
Clay_RenderCommandArray CreateLayout(void) {
    Clay_BeginLayout();

    CLAY({.id = CLAY_ID("OuterContainer"),
          .layout = defaultLayoutStyle,
          .backgroundColor = COLOR_WHITE}) {
        CLAY({.id = CLAY_ID("TopHeaderBar"),
              .layout = CreateLayoutConfigWithHeight(64),
              .backgroundColor = COLOR_GRAY,
              .cornerRadius = {16, 16, 16, 16}}) {
            // Display BLE state
            switch (bleState) {
            case 0:
                CLAY_TEXT(CLAY_STRING("IDLE"), &textConfig);
                break;
            case 1:
                CLAY_TEXT(CLAY_STRING("Scanning..."), &textConfig);
                break;
            case 2:
                CLAY_TEXT(CLAY_STRING("CONNECTED to nRF52840"), &textConfig);
                break;
            default:
                CLAY_TEXT(CLAY_STRING("UNKNOWN State"), &textConfig);
                break;
            }
        }
        CLAY({.id = CLAY_ID("MainBody"),
              .layout = defaultLayoutStyle,
              .backgroundColor = COLOR_BLUE}) {}
    }

    return Clay_EndLayout();
}

void UpdateDrawFrame(Font *fonts) {
    // Add logic for handling events here

    // Build the layout
    Clay_RenderCommandArray renderCommands = CreateLayout();

    // Render the layout
    BeginDrawing();
    ClearBackground(RAYWHITE);
    Clay_Raylib_Render(renderCommands, fonts);
    EndDrawing();
}

int main(void) {

    uint64_t clayMemorySize = Clay_MinMemorySize();
    Clay_Arena memoryArena = Clay_CreateArenaWithCapacityAndMemory(
        clayMemorySize, malloc(clayMemorySize));
    Clay_Dimensions dimensions = {
        .width = screenWidth,
        .height = screenHeight,
    };

    Clay_Initialize(memoryArena, dimensions,
                    (Clay_ErrorHandler){HandleClayErrors});
    Clay_Raylib_Initialize((int)screenWidth, (int)screenHeight,
                           "SpinSat Controller", 0);

    Font fonts[1];
    fonts[0] = LoadFontEx("resources/OpenSans.ttf", 24, 0, 400);
    SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

    initBLE();
    printf("Bluetooth initialized\n");

    startScanning(); // Look for SpinSat BLE signature
    printf("Scanning for devices...\n");

    while (!WindowShouldClose()) {
        Clay_RenderCommandArray renderCommands = CreateLayout();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        Clay_Raylib_Render(renderCommands, fonts);
        EndDrawing();
    }

    Clay_Raylib_Close();
    return 0;
}