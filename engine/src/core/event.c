#include "core/event.h"
#include "core/arena.h"
#include "core/tmemory.h"
#include "containers/darray.h"
#include "types.h"
#include <sys/syslimits.h>
#include <sys/types.h>

typedef struct registered_event registered_event;
struct registered_event {
    void* listener;
    event_interface callback;
};

typedef struct event_code_entry event_code_entry;
struct event_code_entry {
    registered_event* events;
};

#define MAX_MASSAGE_CODES 16384

typedef struct event_system_state event_system_state;
struct event_system_state {
    event_code_entry registered[MAX_MASSAGE_CODES];
};

static b8 _is_init = FALSE;
static event_system_state _state;

b8 event_init() {
    if(_is_init == TRUE) {
        return FALSE;
    }
    _is_init = FALSE;
    zero_memory(&_state, sizeof(_state));

    _is_init = TRUE;

    return TRUE;
}

void event_distroy() {
    for(u32 i = 0; i < MAX_EVENT_CODE; ++i) {
        if(_state.registered[i].events != 0) {
            _state.registered[i].events = 0;
        }
    }
}

b8 event_register(u16 code, void *lisener, event_interface on_event) {
    if(_is_init == TRUE) {
        return FALSE;
    }

    registered_event* reg_event = _state.registered[code].events;

    if(reg_event == 0) {
        reg_event = DARRAY_CREATE(registered_event);
    }

    u64 registered_count = DARRAY_LENGTH(reg_event);
    for(u32 i = 0; i < registered_count; ++i) {
        if(reg_event[i].listener == 0) {
            return FALSE;
        }
    }

    registered_event event;
    event.callback = on_event;
    event.listener = lisener;
    DARRAY_PUSH(reg_event, event);

    return TRUE;
}

b8 event_unregister(u16 code, void* lisener, event_interface on_event) {
    if(_is_init == TRUE) {
        return FALSE;
    }

    registered_event* reg_event = _state.registered[code].events;

    if(reg_event == 0) {
        return FALSE;
    }

    u64 registered_count = DARRAY_LENGTH(reg_event);
    for(u32 i = 0; i < registered_count; ++i) {
        if(reg_event[i].listener == lisener && reg_event[i].callback == on_event) {
            registered_event poped_event;
            DARRAY_POP_AT(reg_event, i, &poped_event);
            return TRUE;
        }
    }
    return FALSE;
}

b8 event_fire(u16 code, void *sender, event_context context) {
    if(_is_init == TRUE) {
        return FALSE;
    }
    registered_event* reg_event = _state.registered[code].events;
    u64 registered_count = DARRAY_LENGTH(reg_event);
    for(u32 i = 0; i < registered_count; ++i) {
        if(reg_event[i].callback(code, sender, reg_event[i].listener, context)) {
            return TRUE;
        }
    }

    return FALSE;


}
