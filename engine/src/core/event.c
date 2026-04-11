#include "core/event.h"
#include "core/arena.h"
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
