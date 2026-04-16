#include "app.h"
#include "arena.h"
#include "platform/platform.h"
#include "types.h"
#include "logger.h"
#include "arena.h"
#include "engine_types.h"
#include "core/event.h"

typedef struct app_state app_state;
struct app_state {
    engine* engine_inst;
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    f64 last_time;
};

static b8 init = FALSE;
static app_state _state;

b8 app_create(mem_arena* arena, engine* engine_inst) {
    if(init) {
        T_ERROR("app_create called more then once");
        return FALSE;
    }

    _state.engine_inst = engine_inst;

    init_logging();

    T_FATAL("A test message: %f", 3.14f);
    T_ERROR("A test message: %f", 3.14f);
    T_WARNING ("A test message: %f", 3.14f);
    T_INFO("A test message: %f", 3.14f);
    T_DEBUG("A test message: %f", 3.14f);
    T_TRACE("A test message: %f", 3.14f);

    _state.is_running = TRUE;
    _state.is_suspended = FALSE;

    if(!event_init()) {
        T_ERROR("Event system failed");
        return FALSE;
    }

    if(!init_platform(arena, &_state.platform, engine_inst->config.name, engine_inst->config.start_pos_x, engine_inst->config.start_pos_y, engine_inst->config.start_width, engine_inst->config.start_height)) {
        return FALSE;
    }

    if(!_state.engine_inst->init(_state.engine_inst)) {
        T_FATAL("Engine failed to initalize");
        return FALSE;
    }

    _state.engine_inst->on_resize(_state.engine_inst, _state.width, _state.height);

    init = TRUE;

    return TRUE;
}
b8 app_run() {
    while(_state.is_running) {
        if(!platform_pump_messages(&_state.platform)) {
            _state.is_running = FALSE;
        }

        if(!_state.is_suspended) {
            if (!_state.engine_inst->update(_state.engine_inst, (f64)0.0)) {
                T_FATAL("Engine update failed, shutting down.");
                _state.is_running = FALSE;
                break;
            }
            if (!_state.engine_inst->render(_state.engine_inst, (f64)0.0)) {
                T_FATAL("Engine render failed, shutting down.");
                _state.is_running = FALSE;
                break;
            }
        }
    }

    _state.is_running = FALSE;

    event_distroy();

    destroy_platform(&_state.platform);

    return TRUE;
}
