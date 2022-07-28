#pragma once

#include "../util/util.h"
#include "../gfx/texture.h"
#include "../gfx/renderer.h"

#define MAX_VERTEX_COUNT 2048
typedef struct Vertex {
    vec3s position;
    vec3s normal;
    vec2s tex_coords;
} Vertex;

typedef struct MeshTexture {
    Texture tex;
    char *type;
    char *path;
} MeshTexture;

#define MAX_INDEX_COUNT 2048
#define MAX_TEXTURE_COUNT 2048
typedef struct Mesh {
    Vertex *vertices;
    u32 vertex_count;

    u32 *indices;
    u32 index_count;

    MeshTexture *textures;
    u32 texture_count;

    VAO vao;
    VBO vbo, ibo;
} Mesh;

Mesh mesh_create(Vertex *vertices, u32 vertex_count, u32 *indices, u32 index_count, MeshTexture *textures, u32 texture_count);
void mesh_render(Mesh *self, ShaderType shader);
