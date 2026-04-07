#include "engine.h"
#include <entry.h>
#include <types.h>
#include <core/arena.h>

b8 create_engine(mem_arena* arena, engine* engine) {
    engine->config.name = "TAKA Engine";
    engine->config.start_height = 2000;
    engine->config.start_width = 2000;
    engine->config.start_pos_x = 100;
    engine->config.start_pos_y = 100;

    engine->init = engine_init;
    engine->render = engine_render;
    engine->update = engine_update;
    engine->on_resize = engine_on_resize;

    engine->state = PUSH_STRUCT_ZERO(arena, engine_state);

    return TRUE;
}
