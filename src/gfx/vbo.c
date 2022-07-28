#include "vbo.h"

VBO vbo_create(GLint type, bool dynamic) {
    VBO self = {
        .type = type,
        .dynamic = dynamic
    };
    glGenBuffers(1, &self.handle);
    return self;
}

void vbo_destroy(VBO self) {
    glDeleteBuffers(1, &self.handle);
}

void vbo_bind(VBO self) {
    glBindBuffer(self.type, self.handle);
}

void vbo_buffer(VBO self, void *data, size_t offset, size_t count) {
    vbo_bind(self);
    glBufferData(self.type, count - offset, data, self.dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}
