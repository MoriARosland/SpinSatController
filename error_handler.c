#include <stdio.h>
#include "error_handler.h"

void HandleClayErrors(Clay_ErrorData errorData)
{
    printf("Clay Error: %s\n", errorData.errorText.chars);

    switch (errorData.errorType)
    {
    case CLAY_ERROR_TYPE_TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED:
        printf("Error: Text measurement function was not provided or is null.\n");
        printf("Fix: Call Clay_SetMeasureTextFunction() with a valid text measurement function.\n");
        break;

    case CLAY_ERROR_TYPE_ARENA_CAPACITY_EXCEEDED:
        printf("Error: Clay ran out of memory in its internal arena.\n");
        printf("Fix: Increase the arena capacity to at least Clay_MinMemorySize() bytes.\n");
        break;

    case CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED:
        printf("Error: Clay ran out of capacity for storing UI elements.\n");
        printf("Fix: Increase the maximum element count using Clay_SetMaxElementCount().\n");
        break;

    case CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED:
        printf("Error: Clay ran out of capacity for text measurement cache.\n");
        printf("Fix: Increase the cache size using Clay_SetMaxMeasureTextCacheWordCount().\n");
        break;

    case CLAY_ERROR_TYPE_DUPLICATE_ID:
        printf("Error: Two UI elements were declared with the same ID.\n");
        printf("Fix: Ensure each element has a unique ID in your layout.\n");
        break;

    case CLAY_ERROR_TYPE_FLOATING_CONTAINER_PARENT_NOT_FOUND:
        printf("Error: A floating element's parent ID was not found.\n");
        printf("Fix: Ensure the parent element exists and is declared before the floating element.\n");
        break;

    case CLAY_ERROR_TYPE_PERCENTAGE_OVER_1:
        printf("Error: An element was declared with a percentage value over 1.\n");
        printf("Fix: Ensure percentage values are in the range 0-1.\n");
        break;

    case CLAY_ERROR_TYPE_INTERNAL_ERROR:
        printf("Error: Clay encountered an internal error.\n");
        printf("Fix: Please report this issue to the Clay development team.\n");
        break;

    default:
        printf("Error: Unknown error type encountered.\n");
        break;
    }
}