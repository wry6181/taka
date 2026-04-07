#pragma once

#include "core/arena.h"
#include "core/app.h"
#include "core/logger.h"
#include "engine_types.h"
#include "types.h"

extern b8 create_engine(mem_arena* arena, engine* out_engine);

int main(void) {

    engine engine_inst;

    mem_arena* arena = arena_create(MByte(100));

    if(!create_engine(arena, &engine_inst)){
        T_ERROR("Could not ceate engine");
        return -1;
    }

    if(!engine_inst.init || !engine_inst.render || !engine_inst.update || !engine_inst.on_resize) {
        T_FATAL("The engine func pointer must be defined");
        return -2;
    }

    if(!app_create(arena, &engine_inst)) {
        T_INFO("Failed to create engine");
        return 1;
    }

    if(!app_run()) {
        T_INFO("Failed to run engine");
        return 2;
    }



}
