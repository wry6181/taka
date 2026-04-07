#pragma once

#include "arena.h"
#include "types.h"

struct engine;

typedef struct app_config app_config;
struct app_config {
    i16 start_pos_x;
    i16 start_pos_y;
    i16 start_width;
    i16 start_height;
    char* name;
};

b8 app_create(mem_arena* arena, struct engine* engine_inst);
b8 app_run();
