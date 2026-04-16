#pragma once

#include "types.h"
#include "core/arena.h"

enum {
    DARRAY_CAPACITY,
    DARRAY_LENGTH,
    DARRAY_STRIDE,
    DARRAY_FIELD_LENGHT,
};


void* darray_create(u64 lenght, u64 stride);
void darray_destroy(void* darray);

u64 darray_field_get(void* darray, u64 field);
void darray_field_set(void* darray, u64 field, u64 value);

void* darray_resize(void* darray);

void* darray_push(void* darray, const void* valu_ptr);
void darray_pop(void* darray, void* dest);

void* darray_insert(void* darray, u64 index, const void* valu_ptr);
void darray_pop_at(void* darray, u64 index, void* dest);

#define DARRAY_DEF_SIZE 1
#define DARRAY_RESIZE_FACTOR 2

#define DARRAY_CREATE(type) \
    darray_create(DARRAY_DEF_SIZE, sizeof(type))

#define DARRAY_RESERVED(type, capacity) \
    darray_create(capacity, sizeof(type))

#define DARRAY_DESTROY(darray) \
    darray_destroy(darray)

#define DARRAY_PUSH(darray, value) \
    {   \
        typeof(value) temp = value; \
        darray_push(darray, &value); \
    }   \

#define DARRAY_POP(darray, value_ptr) \
        darray_pop(darray, value_ptr)

#define DARRAY_PUSH_AT(darray, index, value) \
    {   \
        typeof(value) temp = value; \
        darray_insert(darray, index, &value); \
    }   \

#define DARRAY_POP_AT(darray, index, value_ptr) \
    darray_pop_at(darray, index, value_ptr)

#define DARRAY_CLEAR(darray) \
    darray_field_set(darray, DARRAY_LENGHT, 0)

#define DARRAY_CAPACITY(darray) \
    darray_field_get(darray, DARRAY_CAPACITY)

#define DARRAY_LENGTH(darray) \
    darray_field_get(darray, DARRAY_LENGTH)

#define DARRAY_STRIDE(darray) \
    darray_field_get(darray, DARRAY_STRIDE)

#define DARRAY_LENGTH_SET(darray, lenght) \
    darray_field_set(darray, DARRAY_LENGTH, lenght)
