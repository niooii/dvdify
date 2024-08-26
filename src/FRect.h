#pragma once
#include <Windows.h>
#include "vec2.h"

typedef struct FRect {
    Vec2 top_left;
    float w;
    float h;
} FRect;


FRect frect_from_winrect(RECT in)
{
    // This is struct initialization syntax, very cool
    FRect f = {
        .top_left = {
            .x = in.left,
            .y = in.top
        },
        .w = in.right - in.left,
        .h = in.bottom - in.top
    };
    return f;
}