#include "core/input.h"
#include "core/event.h"
#include "core/tmemory.h"
#include "core/logger.h"

typedef struct keyboard_state {
    b8 keys[256];
} keyboard_state;

typedef struct mouse_state {
    i16 x;
    i16 y;
    u8 buttons[BUTTON_MAX_BUTTONS];
} mouse_state;

typedef struct input_state {
    keyboard_state keyboard_current;
    keyboard_state keyboard_previous;
    mouse_state mouse_current;
    mouse_state mouse_previous;
} input_state;

// Internal input state
static b8 _initialized = FALSE;
static input_state _state = {};

void input_init() {
    zero_memory(&_state, sizeof(input_state));
    _initialized = TRUE;
    T_INFO("Input subsystem initialized.");
}

void input_destroy() {
    // TODO: Add shutdown routines when needed.
    _initialized = FALSE;
}

void input_update(f64 delta_time) {
    if (!_initialized) {
        return;
    }

    // Copy current states to previous states.
    copy_memory(&_state.keyboard_previous, &_state.keyboard_current, sizeof(keyboard_state));
    copy_memory(&_state.mouse_previous, &_state.mouse_current, sizeof(mouse_state));
}

void input_process_key(keys key, b8 pressed) {
    // Only handle this if the state actually changed.
    if (_state.keyboard_current.keys[key] != pressed) {
        // Update internal state.
        _state.keyboard_current.keys[key] = pressed;

        // Fire off an event for immediate processing.
        event_context context;
        context.data.u16[0] = key;
        event_fire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, context);
    }
}

void input_process_button(buttons button, b8 pressed) {
    // If the state changed, fire an event.
    if (_state.mouse_current.buttons[button] != pressed) {
        _state.mouse_current.buttons[button] = pressed;

        // Fire the event.
        event_context context;
        context.data.u16[0] = button;
        event_fire(pressed ? EVENT_CODE_BUTTON_PRESSED : EVENT_CODE_BUTTON_RELEASED, 0, context);
    }
}

void input_process_mouse_move(i16 x, i16 y) {
    // Only process if actually different
    if (_state.mouse_current.x != x || _state.mouse_current.y != y) {
        // NOTE: Enable this if debugging.
        //KDEBUG("Mouse pos: %i, %i!", x, y);

        // Update internal state.
        _state.mouse_current.x = x;
        _state.mouse_current.y = y;

        // Fire the event.
        event_context context;
        context.data.u16[0] = x;
        context.data.u16[1] = y;
        event_fire(EVENT_CODE_MOUSE_MOVED, 0, context);
    }
}

void input_process_mouse_wheel(i8 z_delta) {
    // NOTE: no internal state to update.

    // Fire the event.
    event_context context;
    context.data.u8[0] = z_delta;
    event_fire(EVENT_CODE_MOUSE_WHEEL, 0, context);
}

b8 input_is_key_down(keys key) {
    if (!_initialized) {
        return FALSE;
    }
    return _state.keyboard_current.keys[key] == TRUE;
}

b8 input_is_key_up(keys key) {
    if (!_initialized) {
        return TRUE;
    }
    return _state.keyboard_current.keys[key] == FALSE;
}

b8 input_was_key_down(keys key) {
    if (!_initialized) {
        return FALSE;
    }
    return _state.keyboard_previous.keys[key] == TRUE;
}

b8 input_was_key_up(keys key) {
    if (!_initialized) {
        return TRUE;
    }
    return _state.keyboard_previous.keys[key] == FALSE;
}

// mouse input
b8 input_is_button_down(buttons button) {
    if (!_initialized) {
        return FALSE;
    }
    return _state.mouse_current.buttons[button] == TRUE;
}

b8 input_is_button_up(buttons button) {
    if (!_initialized) {
        return TRUE;
    }
    return _state.mouse_current.buttons[button] == FALSE;
}

b8 input_was_button_down(buttons button) {
    if (!_initialized) {
        return FALSE;
    }
    return _state.mouse_previous.buttons[button] == TRUE;
}

b8 input_was_button_up(buttons button) {
    if (!_initialized) {
        return TRUE;
    }
    return _state.mouse_previous.buttons[button] == FALSE;
}

void input_get_mouse_position(i32* x, i32* y) {
    if (!_initialized) {
        *x = 0;
        *y = 0;
        return;
    }
    *x = _state.mouse_current.x;
    *y = _state.mouse_current.y;
}

void input_get_previous_mouse_position(i32* x, i32* y) {
    if (!_initialized) {
        *x = 0;
        *y = 0;
        return;
    }
    *x = _state.mouse_previous.x;
    *y = _state.mouse_previous.y;
}
