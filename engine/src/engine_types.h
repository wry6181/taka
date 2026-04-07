#pragma once

#include "core/app.h"

typedef struct engine engine;
struct engine {
    app_config config;

    b8(*init)(engine* engine_inst);

    b8(*update)(engine* engine_inst, f32 delta_time);

    b8(*render)(engine* engine_inst, f32 delta_time);

    void(*on_resize)(engine* engine_inst, u32 width, u32 height);

    void* state;
};
