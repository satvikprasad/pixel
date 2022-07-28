#include "world.h"
#include "../state.h"
#include "cglm/struct/mat4.h"
#include "cglm/struct/vec3.h"

void world_init(World *self) {
    self->vao = vao_create();
    self->vbo = vbo_create(GL_ARRAY_BUFFER, true);
    self->ibo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, true);

    self->point_lights[0] = (PointLight){
        .ambient = GLMS_VEC3_ZERO,
        .diffuse = GLMS_VEC3_ZERO,
        .specular = GLMS_VEC3_ONE,
        .position = GLMS_VEC3_ONE,
        .constant = 1.f,
        .linear = 0.0035f,
        .quadratic = 0.0032f,
    };

    self->point_lights[1] = (PointLight) {
        .ambient = (vec3s){{ 0.1f, 0.1f, 0.1f }},
        .diffuse = GLMS_VEC3_ONE,
        .specular = GLMS_VEC3_ONE,
        .position = (vec3s){{0.0f, 0.f, -10.f}},
        .constant = 1.f,
        .linear = 0.0025f,
        .quadratic = 0.0032f,
    };
   
    self->dir_light = (DirLight) {
        .direction = (vec3s){{ -0.2f, -1.0f, 0.3f }},
        .ambient = (vec3s){{ 0.1f, 0.1f, 0.1f }},
        .diffuse = (vec3s){{ 0.4f, 0.4f, 0.4f }},
    };

    self->player_mat = (Material){
        .diffuse = (vec3s) {{0.9f, 0.9, 0.9}},
        .specular = (vec3s) {{0.6f, 0.6f, 0.6f}},
        .shine = 560,
    };
}

void world_destroy(World *self) {
    vbo_destroy(self->vbo);
    vbo_destroy(self->ibo);
    vao_destroy(self->vao);
}

void world_update(World *self) { 
    f32 sens = 1.f;
    f32 speed = 0.05f;

    vec2s mag = glms_vec2_div(
        glms_vec2_sub(IVEC2S2V(window.size), window.mouse.position),
        IVEC2S2V(window.size));
    
    vec3s dir = state.renderer.perspective_camera.direction;
    vec3s vel = GLMS_VEC3_ZERO;

    if (window.keyboard.keys[GLFW_KEY_W].down) {
        vel = glms_vec3_add(dir, vel);
    }
    if (window.keyboard.keys[GLFW_KEY_S].down) {
        vel = glms_vec3_add(glms_vec3_negate(dir), vel);
    }
    if (window.keyboard.keys[GLFW_KEY_D].down) {
        vec3s dir_perp = (vec3s){{-dir.z, dir.y, dir.x}};
        vel = glms_vec3_add(dir_perp, vel);
    }
    if (window.keyboard.keys[GLFW_KEY_A].down) {
        vec3s dir_perp = (vec3s){{dir.z, dir.y, -dir.x}};
        vel = glms_vec3_add(dir_perp, vel);
    }
    if (window.keyboard.keys[GLFW_KEY_SPACE].down) {
        vel = glms_vec3_add((vec3s){{0.0f, 1.0f, 0.0f}}, vel);
    }
    if (window.keyboard.keys[GLFW_KEY_LEFT_SHIFT].down) {
        vel = glms_vec3_add((vec3s){{0.0f, -1.0f, 0.0f}}, vel);
    }

    vel = glms_vec3_scale(glms_normalize(vel), speed);

    state.renderer.perspective_camera.yaw = mag.x * sens;
    state.renderer.perspective_camera.pitch = mag.y * sens;
    self->player_position =
        glms_vec3_add(self->player_position, vel);

    state.renderer.perspective_camera.position = glms_vec3_sub(self->player_position, glms_vec3_scale(dir, 2.f));

    perspective_camera_update(&state.renderer.perspective_camera);

    static f32 i = 0.f;
    self->point_lights[0].position.x = 5*sinf(i);
    i += 0.01f;

    self->point_lights[0].diffuse.x = 0.5f * sin(0.01f * state.ticks * 2.0f);
    self->point_lights[0].diffuse.y = 0.5f * sin(0.01f * state.ticks * 0.7f);
    self->point_lights[0].diffuse.z = 0.5f * sin(0.01f * state.ticks * 1.3f);
    self->point_lights[0].ambient = glms_vec3_scale(self->point_lights[0].diffuse, 0.4);
}

void world_render_cube(World *self, vec3s size, Material mat, mat4s model) {
    renderer_use_shader(&state.renderer, ShaderType_phong);
    renderer_set_view_proj(&state.renderer);
    shader_uniform_mat4(state.renderer.shader, "m", model);
    
    shader_uniform_material(state.renderer.shader, mat);

    for (u32 i = 0; i < 2; ++i) 
        shader_uniform_point_light(state.renderer.shader, self->point_lights[i], i);

    shader_uniform_int(state.renderer.shader, "point_light_count", 2);

    shader_uniform_vec3(state.renderer.shader, "dir_light.ambient", self->dir_light.ambient);
    shader_uniform_vec3(state.renderer.shader, "dir_light.diffuse", self->dir_light.diffuse);
    shader_uniform_vec3(state.renderer.shader, "dir_light.specular", self->dir_light.specular);
    shader_uniform_vec3(state.renderer.shader, "dir_light.direction", self->dir_light.direction);

    shader_uniform_vec3(state.renderer.shader, "view_pos", state.renderer.perspective_camera.position);
    
    // toon shader
    // shader_uniform_float(state.renderer.shader, "n_shades", 10.f);

    vec3s half_size = glms_vec3_scale(size, 0.5f);

    vbo_buffer(self->vbo,
               (f32[]){
                   half_size.x,  half_size.y,  -half_size.z, 0.0f,  0.0f,  -1.0f, // 2
                   half_size.x,  -half_size.y, -half_size.z, 0.0f,  0.0f,  -1.0f, // 1
                   -half_size.x, -half_size.y, -half_size.z, 0.0f,  0.0f,  -1.0f, // 0
                   -half_size.x, half_size.y,  -half_size.z, 0.0f,  0.0f,  -1.0f, // 4

                   -half_size.x, -half_size.y, half_size.z,  0.0f,  0.0f,  1.0f, // 6
                   half_size.x,  -half_size.y, half_size.z,  0.0f,  0.0f,  1.0f, // 7
                   half_size.x,  half_size.y,  half_size.z,  0.0f,  0.0f,  1.0f, // 8
                   -half_size.x, half_size.y,  half_size.z,  0.0f,  0.0f,  1.0f, // 10

                   -half_size.x, half_size.y,  half_size.z,  -1.0f, 0.0f,  0.0f, // 12
                   -half_size.x, half_size.y,  -half_size.z, -1.0f, 0.0f,  0.0f, // 13
                   -half_size.x, -half_size.y, -half_size.z, -1.0f, 0.0f,  0.0f, // 14
                   -half_size.x, -half_size.y, half_size.z,  -1.0f, 0.0f,  0.0f, // 16

                   half_size.x,  -half_size.y, -half_size.z, 1.0f,  0.0f,  0.0f, // 20
                   half_size.x,  half_size.y,  -half_size.z, 1.0f,  0.0f,  0.0f, // 19
                   half_size.x,  half_size.y,  half_size.z,  1.0f,  0.0f,  0.0f, // 18
                   half_size.x,  -half_size.y, half_size.z,  1.0f,  0.0f,  0.0f, // 22

                   -half_size.x, -half_size.y, -half_size.z, 0.0f,  -1.0f, 0.0f, // 24
                   half_size.x,  -half_size.y, -half_size.z, 0.0f,  -1.0f, 0.0f, // 25
                   half_size.x,  -half_size.y, half_size.z,  0.0f,  -1.0f, 0.0f, // 26
                   -half_size.x, -half_size.y, half_size.z,  0.0f,  -1.0f, 0.0f, // 28

                   half_size.x,  half_size.y,  half_size.z,  0.0f,  1.0f,  0.0f, // 32
                   half_size.x,  half_size.y,  -half_size.z, 0.0f,  1.0f,  0.0f, // 31
                   -half_size.x, half_size.y,  -half_size.z, 0.0f,  1.0f,  0.0f, // 30
                   -half_size.x, half_size.y,  half_size.z,  0.0f,  1.0f,  0.0f, // 34
               },
               0, 6 * 24 * sizeof(f32));

    vbo_buffer(self->ibo, (u32[]) {
                0,   1,  2,  2,  3, 0,
                4,   5,  6,  6,  7, 4,
                8,   9, 10, 10, 11, 8,
                12, 13, 14, 14, 15, 12,
                16, 17, 18, 18, 19, 16,
                20, 21, 22, 22, 23, 20
            }, 0, 36 * sizeof(u32));

    vao_attr(self->vao, self->vbo, 0, 3, GL_FLOAT,
             6 * sizeof(f32), 0);
    vao_attr(self->vao, self->vbo, 1, 3, GL_FLOAT,
             6 * sizeof(f32), 3 * sizeof(f32));

    vao_bind(self->vao);
    vbo_bind(self->ibo);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void world_render(World *self) {
    for (u32 i = 0; i < 2; ++i) {
     renderer_cube(&state.renderer, (vec3s){{0.5f, 0.5f, 0.5f}}, 
                   VEC3S2VEC4S(self->point_lights[i].diffuse, 1.0f), 
                   glms_translate(glms_mat4_identity(), self->point_lights[i].position));
    }

    world_render_cube(self, (vec3s){{ 0.5f, 0.5f, 0.5f }}, (Material) {
                (vec3s){{0.4, 0.1, 0.1}}, 
                GLMS_VEC3_ONE,
                200
            }, glms_translate(glms_mat4_identity(), (vec3s) {{0.f, 0.f, 3.f}}));
    world_render_cube(self, (vec3s){{ 0.5f, 0.5f, 0.5f }}, (Material) {
                (vec3s){{0.4, 0.1, 0.1}}, 
                GLMS_VEC3_ONE,
                200
            }, glms_translate(glms_mat4_identity(), (vec3s) {{3.f, 15.f, 3.f}}));
    world_render_cube(self, (vec3s){{ 200.f, 1.f, 200.f }}, (Material) {
                (vec3s){{0.1, 0.1, 0.1}}, 
                GLMS_VEC3_ONE,
                800
            }, glms_translate(glms_mat4_identity(), (vec3s) {{0.f, -1.f, 0.f}}));

    world_render_cube(self, (vec3s){{ 0.5f, 0.5f, 0.5f }}, self->player_mat, glms_translate(glms_mat4_identity(), self->player_position));

    Vertex v1 = (Vertex){{ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f }};
    Vertex v2 = (Vertex){{ 1.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f }};
    Vertex v3 = (Vertex){{ 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f }};
}
