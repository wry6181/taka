#pragma onece

#include <types.h>
#include <engine_types.h>

typedef struct engine_state engine_state;
struct engine_state {
    f32 delta_time;
};

b8 engine_init(engine* engine_inst);

b8 engine_render(engine* engine_inst, f32 delta_time);

b8 engine_update(engine* engine_inst, f32 delta_time);

void engine_on_resize(engine* engine_inst, u32 width, u32 height);
