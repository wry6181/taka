#include "engine.h"

#include <core/logger.h>

b8 engine_init(engine* engine_inst){
    T_DEBUG("Init works");
    return TRUE;
}

b8 engine_render(engine* engine_inst, f32 delta_time){
    return TRUE;
}

b8 engine_update(engine* engine_inst, f32 delta_time){
    return TRUE;
}

void engine_on_resize(engine* engine_inst, u32 width, u32 height){}
