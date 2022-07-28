#pragma once 

#include "../util/util.h"
#include "../gfx/material.h"
#include "mesh.h"
#include "../gfx/light.h"

typedef struct World {
    vec3s player_position;       
    Material player_mat;
    PointLight point_lights[2];
    DirLight dir_light;

    VAO vao;
    VBO vbo, ibo;
} World;

void world_init(World *self);
void world_update(World *self);
void world_render(World *self);
