#include "physics_2d.h"

void physics2d_init(Physics2D* sim)
{
    sim->simulated_rect_count = 0;
    sim->timescale = 1.f;
    sim->simulated_rects = NULL;
}

void physics2d_set_simulation_area(Physics2D* sim, FRect simulation_area)
{
    sim->simulation_area = simulation_area;
}

void physics2d_step(Physics2D* sim, double dt)
{
    dt = dt * sim->timescale;
    for (unsigned int i = 0; i < sim->simulated_rect_count; i++)
    {
        PhysicsObject2D* obj = sim->simulated_rects[i];
        Vec2* v = &obj->vel;
        Vec2* a = &obj->accel;
        FRect* rect = &obj->collider;
        FRect* sim_area_rect = &sim->simulation_area;

        // Kinematic shit
        float dx = v->x * dt + (1/2.f) * a->x * dt * dt;
        float dy = v->y * dt + (1/2.f) * a->y * dt * dt;
        v->x = v->x + a->x * dt;
        v->y = v->y + a->y * dt;

        frect_translate(rect, dx, dy);

        // Keep objects within simulation area
        if (rect->left <= sim_area_rect->left)
        {
            rect->left = sim_area_rect->left;
            v->x *= -1;
        }
        if (rect->right >= sim_area_rect->right)
        {
            rect->right = sim_area_rect->right;
            v->x *= -1;
        }
        if (rect->top <= sim_area_rect->top)
        {
            rect->top = sim_area_rect->top;
            v->y *= -1;
        }
        if (rect->bottom >= sim_area_rect->bottom)
        {
            rect->bottom = sim_area_rect->bottom;
            v->y *= -1;
        }

        // TODO! handle collisions between colliders
    }
}

void physics2d_add_object(Physics2D* sim, PhysicsObject2D* object)
{
    sim->simulated_rect_count++;
    sim->simulated_rects = realloc(
        sim->simulated_rects, 
        sim->simulated_rect_count * sizeof(object)
    );
    sim->simulated_rects[sim->simulated_rect_count - 1] = object;
}

void physics2d_set_timescale(Physics2D* sim, float timescale)
{
    sim->timescale = timescale;
}