#include "physics_2d.h"

void physics2d_init(Physics2D* out_physics, FRect simulation_area)
{
    out_physics->simulation_area = simulation_area;
}
void physics2d_step(Physics2D* physics, double dt)
{
    for (unsigned int i = 0; i < physics->simulated_rect_count; i++)
    {
        PhysicsObject2D* obj = physics->simulated_rects[i];
        Vec2* v = &obj->vel;
        Vec2* a = &obj->accel;
        FRect* rect = &obj->collider;
        FRect* sim_area_rect = &physics->simulation_area;
        // Kinematic shit
        float dx = v->x * dt + (1/2.f) * a->x * dt * dt;
        float dy = v->y * dt + (1/2.f) * a->y * dt * dt;
        v->x = v->x + a->x * dt;
        v->y = v->y + a->y * dt;
        frect_translate(rect, dx, dy);

        // basic collision system
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
    }
}
void physics2d_add_object(Physics2D* physics, PhysicsObject2D* object)
{
    physics->simulated_rect_count++;
    physics->simulated_rects = realloc(
        physics->simulated_rects, 
        physics->simulated_rect_count * sizeof(object)
    );
    physics->simulated_rects[physics->simulated_rect_count - 1] = object;
}