#ifndef WINDOW_H
#define WINDOW_H

#include "gfx.h"
#include "../util/util.h"

typedef struct Button {
    bool down, last, last_tick, pressed, pressed_tick;
} Button;


typedef struct Mouse {
    Button buttons[GLFW_MOUSE_BUTTON_LAST];
    vec2s position, delta;
} Mouse;

typedef struct Keyboard {
    Button keys[GLFW_KEY_LAST];
} Keyboard;

typedef void (*FWindow)();

typedef struct Window {
    GLFWwindow *handle;
    ivec2s size;
    FWindow init, destroy, tick, update, render;
    Mouse mouse;
    Keyboard keyboard;

    // timing variables
    u64 last_second;
    u64 frames, fps, last_frame, frame_delta;
    u64 ticks, tps, tick_remainder;
} Window;

// global window
extern Window window;

void window_loop();
void window_create(FWindow init, FWindow destroy, FWindow tick,  FWindow update, FWindow render);
void mouse_set_grabbed(bool grabbed);
bool mouse_get_grabbed();

#endif
