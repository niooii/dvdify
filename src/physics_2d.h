#pragma once

#include <Windows.h>
#include <stdlib.h>
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

inline void frect_translate(FRect* rect, float dx, float dy) 
{
    rect->top += dy;
    rect->bottom += dy;
    rect->left += dx;
    rect->right += dx;
}

typedef struct PhysicsObject2D {
    FRect collider;
    Vec2 vel;
    Vec2 accel;
} PhysicsObject2D;

typedef struct Physics2D {
    // Dynamic list of rectangle pointers
    PhysicsObject2D** simulated_rects;
    unsigned int simulated_rect_count;
    FRect simulation_area;

    float timescale;
} Physics2D;

void physics2d_init(Physics2D* sim);
void physics2d_set_simulation_area(Physics2D* sim, FRect simulation_area);
void physics2d_step(Physics2D* sim, double delta_time);
void physics2d_add_object(Physics2D* sim, PhysicsObject2D* object);
void physics2d_set_timescale(Physics2D* sim, float timescale);