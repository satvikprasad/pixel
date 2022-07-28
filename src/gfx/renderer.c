#include "../state.h"
#include "cglm/struct/mat4.h"
#include "renderer.h"

void renderer_init(Renderer *self, vec2s resolution) {
    *self = (Renderer){0};
    self->camera_type = CameraType_perspective;
    self->current_shader = ShaderType_none;
    self->render_res = resolution;

    self->shaders[ShaderType_basic_texture] = shader_create(
        "res/shaders/basic_texture.vs", "res/shaders/basic_texture.fs", 2,
        (VertexAttr[]){{.index = 0, .name = "position"},
                       {.index = 1, .name = "uv"}});

    self->shaders[ShaderType_chunk] =
        shader_create("res/shaders/chunk.vs", "res/shaders/chunk.fs", 3,
                      (VertexAttr[]){{.index = 0, .name = "position"},
                                     {.index = 1, .name = "uv"},
                                     {.index = 2, .name = "color"}});

    self->shaders[ShaderType_sky] =
        shader_create("res/shaders/sky.vs", "res/shaders/sky.fs", 2,
                      (VertexAttr[]){{.index = 0, .name = "position"},
                                     {.index = 1, .name = "uv"}});

    self->shaders[ShaderType_basic_color] = shader_create(
        "res/shaders/basic_color.vs", "res/shaders/basic_color.fs", 1,
        (VertexAttr[]){{.index = 0, .name = "position"}});

    self->shaders[ShaderType_toon] =
        shader_create("res/shaders/toon.vs", "res/shaders/toon.fs", 2,
                      (VertexAttr[]){{.index = 0, .name = "position"},
                                     {.index = 1, .name = "normal"}});

    self->shaders[ShaderType_phong] =
        shader_create("res/shaders/phong.vs", "res/shaders/phong.fs", 2,
                      (VertexAttr[]){{.index = 0, .name = "position"},
                                     {.index = 1, .name = "normal"}});

    self->vao = vao_create();
    self->vbo = vbo_create(GL_ARRAY_BUFFER, true);
    self->ibo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, true);
    
    self->render_fbo = fbo_create();
    self->render_rbo = rbo_create(GL_DEPTH_COMPONENT, self->render_res);

    self->render_texture =
        texture_create_empty(self->render_res.x, self->render_res.y);

    fbo_rbo(self->render_fbo, self->render_rbo, GL_DEPTH_ATTACHMENT);
    fbo_texture(self->render_fbo, self->render_texture);

    self->depth_map_fbo = fbo_create();
  
    glGenTextures(1, &self->depth_map);
    glBindTexture(GL_TEXTURE_2D, self->depth_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    fbo_bind(self->depth_map_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, self->depth_map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    perspective_camera_init(&self->perspective_camera, radians(75.0f));
    ortho_camera_init(&self->ortho_camera, GLMS_VEC2_ZERO,
                      (vec2s){{state.window->size.x, state.window->size.y}});

    glViewport(0, 0, self->render_res.x, self->render_res.y);
}

void renderer_destroy(Renderer *self) {
    for (size_t i = 0; i < ShaderType_MAX; i++) {
        shader_destroy(self->shaders[i]);
    }

    vao_destroy(self->vao);
    vbo_destroy(self->vbo);
    vbo_destroy(self->ibo);
    fbo_destroy(self->render_fbo);
    fbo_destroy(self->depth_map_fbo);
}

void renderer_update(Renderer *self) {}

// TODO: change this to two passes, make world_render take a shader value to use simple_depth_shader
void renderer_begin(Renderer *self) { 
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    fbo_bind(self->depth_map_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    {
        mat4s light_projection = glms_ortho(-10.f, 10.f, -10.f, 10.f, 1.0f, 7.5f);
        mat4s light_view = glms_lookat((vec3s) {{-2.0f, 4.0f, -1.0f}}, 
                                       GLMS_VEC3_ZERO,
                                       GLMS_VEC3_ZERO);
        mat4s light_space_matrix = glms_mat4_mul(light_projection, light_view);
    }
    fbo_bind(self->render_fbo);
    glViewport(0, 0, self->render_res.x, self->render_res.y);
}

void renderer_blit(Renderer *self) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, self->render_res.x, self->render_res.y, 0, 0,
                      window.size.x, window.size.y, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
                      GL_NEAREST);
}

void renderer_prepare(Renderer *self, enum RenderPass pass) {
    switch (pass) {
    case RenderPass_2d:
        ortho_camera_init(
            &self->ortho_camera, GLMS_VEC2_ZERO,
            (vec2s){{state.window->size.x, state.window->size.y}});
        glClear(GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_MULTISAMPLE);
        break;
    case RenderPass_3d:
        glClearColor(self->clear_color.x, self->clear_color.y,
                     self->clear_color.z, self->clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK,
                      self->flags.wireframe ? GL_LINE : GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_MULTISAMPLE);
        break;
    }
}

void renderer_set_camera(Renderer *self, enum CameraType type) {
    self->camera_type = type;
}

void renderer_push_camera(Renderer *self) {
    assert(self->camera_stack.size + 1 < CAMERA_STACK_MAX);
    self->camera_stack.array[self->camera_stack.size++] = self->camera_type;
}

void renderer_pop_camera(Renderer *self) {
    assert(self->camera_stack.size != 0);
    self->camera_type = self->camera_stack.array[--self->camera_stack.size];
}

void renderer_set_view_proj(Renderer *self) {
    ViewProj view_proj;

    switch (self->camera_type) {
    case CameraType_perspective:
        view_proj = self->perspective_camera.view_proj;
        break;
    case CameraType_ortho:
        view_proj = self->ortho_camera.view_proj;
        break;
    default:
        break;
    }

    shader_uniform_view_proj(self->shader, view_proj);
}

void renderer_use_shader(Renderer *self, enum ShaderType shader) {
    if (shader == self->current_shader) {
        return;
    }

    self->current_shader = shader;
    self->shader = self->shaders[shader];
    shader_bind(self->shader);
}

void renderer_quad_color(Renderer *self, vec2s size, vec4s color, mat4s model) {
    renderer_use_shader(self, ShaderType_basic_color);
    renderer_set_view_proj(self);
    shader_uniform_mat4(self->shader, "m", model);
    shader_uniform_vec4(self->shader, "color", color);

    vbo_buffer(self->vbo,
               (f32[]){
                   0,
                   0,
                   0,
                   0,
                   size.y,
                   0,
                   size.x,
                   size.y,
                   0,
                   size.x,
                   0,
                   0,
               },
               0, (4 * 3) * sizeof(f32));

    vbo_buffer(self->ibo, (u32[]){3, 0, 1, 3, 1, 2}, 0, 6 * sizeof(u32));
    vao_attr(self->vao, self->vbo, 0, 3, GL_FLOAT, 0, 0);

    vao_bind(self->vao);
    vbo_bind(self->ibo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);
}

void renderer_quad_texture(Renderer *self, Texture texture, vec2s size,
                           vec4s color, vec2s uv_min, vec2s uv_max,
                           mat4s model) {
    renderer_use_shader(self, ShaderType_basic_texture);
    renderer_set_view_proj(self);
    shader_uniform_mat4(self->shader, "m", model);
    shader_uniform_texture2D(self->shader, "tex", texture, 0);
    shader_uniform_vec4(self->shader, "color", color);

    vbo_buffer(self->vbo, (f32[]){0,        0,        0,        0,
                                  size.y,   0,        size.x,   size.y,
                                  0,        size.x,   0,        0,

                                  uv_min.x, uv_min.y, uv_min.x, uv_max.y,
                                  uv_max.x, uv_max.y, uv_max.x, uv_min.y},
               0, ((4 * 3) + (4 * 2)) * sizeof(f32));

    vbo_buffer(self->ibo, (u32[]){3, 0, 1, 3, 1, 2}, 0, 6 * sizeof(u32));

    vao_attr(self->vao, self->vbo, 0, 3, GL_FLOAT, 0, 0);
    vao_attr(self->vao, self->vbo, 1, 2, GL_FLOAT, 0, (4 * 3) * sizeof(f32));

    vao_bind(self->vao);
    vbo_bind(self->ibo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);
}

void renderer_aabb(Renderer *self, AABB aabb, vec4s color, mat4s model,
                   enum FillMode fill_mode) {
    renderer_use_shader(self, ShaderType_basic_color);
    renderer_set_view_proj(self);
    shader_uniform_mat4(self->shader, "m", model);
    shader_uniform_vec4(self->shader, "color", color);

    u32 indices[] = {
        1, 0, 3, 1, 3, 2, // north (-z)
        4, 5, 6, 4, 6, 7, // south (+z)
        5, 1, 2, 5, 2, 6, // east (+x)
        0, 4, 7, 0, 7, 3, // west (-x)
        2, 3, 7, 2, 7, 6, // top (+y)
        5, 4, 0, 5, 0, 1  // bottom (-y)
    };

    vec3s min = aabb[0], max = aabb[1];
    f32 vertices[] = {
        min.x, min.y, min.z, max.x, min.y, min.z,
        max.x, max.y, min.z, min.x, max.y, min.z,

        min.x, min.y, max.z, max.x, min.y, max.z,
        max.x, max.y, max.z, min.x, max.y, max.z,
    };

    vbo_buffer(self->vbo, vertices, 0, (8 * 3) * sizeof(f32));
    vbo_buffer(self->ibo, indices, 0, 36 * sizeof(u32));

    vao_attr(self->vao, self->vbo, 0, 3, GL_FLOAT, 0, 0);

    vao_bind(self->vao);
    vbo_bind(self->ibo);

    if (fill_mode == FillMode_line) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)0);

    if (fill_mode == FillMode_line) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void renderer_cube(Renderer *self, vec3s size, vec4s color, mat4s model) {
    renderer_use_shader(self, ShaderType_basic_color);
    renderer_set_view_proj(self);
    shader_uniform_mat4(self->shader, "m", model);
    shader_uniform_vec4(self->shader, "color", color);

    vbo_buffer(self->vbo,
               (f32[]){
                   -size.x, -size.y, size.z,  // 0
                   size.x,  -size.y, size.z,  // 1
                   -size.x, size.y,  size.z,  // 2
                   size.x,  size.y,  size.z,  // 3
                   -size.x, -size.y, -size.z, // 4
                   size.x,  -size.y, -size.z, // 5
                   -size.x, size.y,  -size.z, // 6
                   size.x,  size.y,  -size.z  // 7
               },
               0, 3 * 8 * sizeof(f32));
    vbo_buffer(self->ibo,
               (u32[]){// Top
                       7, 6, 2, 2, 3, 7,

                       // Bottom
                       0, 4, 5, 5, 1, 0,

                       // Right
                       0, 2, 6, 6, 4, 0,

                       // Left
                       7, 3, 1, 1, 5, 7,

                       // Back
                       3, 2, 0, 0, 1, 3,

                       // Back
                       4, 6, 7, 7, 5, 4},
               0, 6 * 6 * sizeof(u32));

    vao_attr(self->vao, self->vbo, 0, 3, GL_FLOAT, 0, 0);

    vao_bind(self->vao);
    vbo_bind(self->ibo);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)0);
}
