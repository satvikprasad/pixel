#ifndef RENDERER_H
#define RENDERER_H

#include "../util/util.h"
#include "fbo.h"
#include "rbo.h"
#include "gfx.h"
#include "shader.h"
#include "vao.h"
#include "vbo.h"

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

typedef enum FillMode {
    FillMode_fill,
    FillMode_line,
} FillMode;

typedef enum ShaderType {
    ShaderType_none = 0,    
    ShaderType_chunk,
    ShaderType_sky,
    ShaderType_basic_texture,
    ShaderType_basic_color,
    ShaderType_toon,
    ShaderType_phong,
    ShaderType_MAX,
} ShaderType;

typedef enum Textures { Texture_MAX } Textures;

#define CAMERA_STACK_MAX 256
typedef enum RenderPass { RenderPass_2d, RenderPass_3d } RenderPass;

typedef struct Renderer {
    enum CameraType camera_type;
    struct {
        enum CameraType array[CAMERA_STACK_MAX];
        size_t size;
    } camera_stack;

    PerspectiveCamera perspective_camera;
    OrthoCamera ortho_camera;

    Shader shaders[ShaderType_MAX];
    ShaderType current_shader;
    Shader shader;

    Texture textures[Texture_MAX];

    vec4s clear_color;
    vec2s render_res;

    VBO vbo, ibo;
    VAO vao;
    FBO render_fbo;
    RBO render_rbo;
    
    FBO depth_map_fbo;
    u32 depth_map; 

    Texture render_texture;

    struct {
        bool wireframe : 1;
    } flags;
} Renderer;

void renderer_init(Renderer *self, vec2s resolution);
void renderer_destroy(Renderer *self);
void renderer_update(Renderer *self);
void renderer_begin(Renderer *self);
void renderer_blit(Renderer *self);
void renderer_prepare(Renderer *self, RenderPass pass);
void renderer_set_camera(Renderer *self, CameraType type);
void renderer_push_camera(Renderer *self);
void renderer_pop_camera(Renderer *self);
void renderer_set_view_proj(Renderer *self);
void renderer_use_shader(Renderer *self, ShaderType shader);

void renderer_quad_color(Renderer *self, vec2s size, vec4s color, mat4s model);

void renderer_quad_texture(Renderer *self, Texture texture, vec2s size,
                                                     vec4s color, vec2s uv_min, vec2s uv_max,
                                                     mat4s model);

void renderer_aabb(Renderer *self, AABB aabb, vec4s color, mat4s model,
                                     FillMode fill_mode);
void renderer_cube(Renderer *self, vec3s size, vec4s color, mat4s model);

#endif
