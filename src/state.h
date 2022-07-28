#pragma once

#include "gfx/window.h"
#include "gfx/renderer.h"
#include "world/world.h"

typedef struct State {
	Window *window;
	Renderer renderer;
	World world;
	u64 ticks;
} State;

// global state
extern State state;

