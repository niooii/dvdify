#include "physics_2d.h"

void physics2d_init(Physics2D* out_physics, FRect simulation_area);
void physics2d_step(Physics2D* physics, double delta_time);
void physics2d_add_object(Physics2D* physics, FRect* object);