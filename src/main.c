#include "gfx/gfx.h"
#include "state.h"

// global state
State state;

void init() {
    state.window = &window;

    renderer_init(&state.renderer, (vec2s){{1280, 720}});
    world_init(&state.world);
    state.renderer.clear_color = (vec4s){{0.7, 0.7f, 0.7f, 1.f}};

    mouse_set_grabbed(true);
}

void destroy() { renderer_destroy(&state.renderer); }

void tick() { state.ticks++; }

void update() {
    renderer_update(&state.renderer);
    world_update(&state.world);
}

void render() {
    renderer_begin(&state.renderer);
    {
        renderer_prepare(&state.renderer, RenderPass_3d);
        renderer_push_camera(&state.renderer);
        {
            renderer_set_camera(&state.renderer, CameraType_perspective);
            world_render(&state.world);
        }
        renderer_pop_camera(&state.renderer);

        renderer_prepare(&state.renderer, RenderPass_2d);
        renderer_push_camera(&state.renderer);
        {
            renderer_set_camera(&state.renderer, CameraType_ortho);
        }
        renderer_pop_camera(&state.renderer);
    }
    renderer_blit(&state.renderer);
}

int main(int argc, char *argv[]) {
    window_create(init, destroy, tick, update, render);
    window_loop();
}
