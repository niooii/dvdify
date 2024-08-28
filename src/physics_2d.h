#pragma once

#include <Windows.h>
#include "vec2.h"

typedef struct FRect {
    float top;
    float bottom;
    float left;
    float right;
} FRect;


inline FRect frect_from_winrect(RECT in)
{
    // This is struct initialization syntax, very cool
    FRect f = {
        .top = in.top,
        .bottom = in.bottom,
        .left = in.left,
        .right = in.right,
    };
    return f;
}

typedef struct Physics2D {
    // Dynamic list of rectangle pointers
    FRect** simulated_rects;
    FRect simulation_area;
} Physics2D;

void physics2d_init(Physics2D* out_physics, FRect simulation_area);
void physics2d_step(Physics2D* physics, double delta_time);
void physics2d_add_object(Physics2D* physics, FRect* object);