#include "mesh.h"
#include "../state.h"
#include "cglm/struct/vec4.h"

#include <string.h>

Mesh mesh_create(Vertex *vertices, u32 vertex_count, 
                 u32 *indices, u32 index_count, 
                 MeshTexture *textures, u32 texture_count) {
    Mesh self = {
        .vertices = vertices,
        .indices = indices,
        .textures = textures,
        .vertex_count = vertex_count,
        .index_count = index_count,
        .texture_count = texture_count,
    }; 

    self.vao = vao_create();
    self.vbo = vbo_create(GL_ARRAY_BUFFER, false);
    self.ibo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, false);

    vbo_buffer(self.vbo, vertices, 0, vertex_count * sizeof(Vertex));
    vbo_buffer(self.ibo, indices, 0, index_count * sizeof(u32));
    
    vao_attr(self.vao, self.vbo, 0, 3, GL_FLOAT, sizeof(Vertex), 0);
    vao_attr(self.vao, self.vbo, 1, 3, GL_FLOAT, sizeof(Vertex), sizeof(vec3s));
    vao_attr(self.vao, self.vbo, 2, 3, GL_FLOAT, sizeof(Vertex), 2*sizeof(vec3s));
        
    return self;
}

void mesh_render(Mesh *self, ShaderType shader) {
    renderer_use_shader(&state.renderer, shader);
    renderer_set_view_proj(&state.renderer);
    shader_uniform_mat4(state.renderer.shader, "m", glms_mat4_identity());
    shader_uniform_vec4(state.renderer.shader, "color", GLMS_VEC4_BLACK);

    u32 n_diffuse = 1;
    u32 n_specular = 1;
    for (u32 i = 0; i < self->texture_count; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        char number[8];
        char *name = self->textures[i].type;
        if (strcmp(name, "texture_diffuse") == 0)
            snprintf(number, 8, "%u", n_diffuse++);
        else if (strcmp(name, "texture_specular") == 0)
            snprintf(number, 8, "%u", n_specular++);
    
        char buffer[512];
        snprintf(buffer, 512, "material.%s%s", name, number);
        shader_uniform_int(state.renderer.shader, buffer, i);
        texture_bind(self->textures[i].tex);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    vao_bind(self->vao);
    vbo_bind(self->ibo);
    
    glDrawElements(GL_TRIANGLES, self->index_count, GL_UNSIGNED_INT, 0);
}
