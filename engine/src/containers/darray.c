#include "darray.h"
#include "core/arena.h"
#include "core/logger.h"
#include "core/tmemory.h"
#include "types.h"

void* darray_create(u64 lenght, u64 stride) {
    u64 header_size = DARRAY_FIELD_LENGHT * sizeof(u64);
    u64 size = lenght * stride;
    u64* array = tallocate(header_size + size, MEMORY_TAG_DARRAY);
    set_memory(array, 0, header_size + size);
    array[DARRAY_LENGTH] = 0;
    array[DARRAY_CAPACITY] = lenght;
    array[DARRAY_STRIDE] = stride;
    return (void*)(array + DARRAY_FIELD_LENGHT);
}

void darray_destroy(void* darray) {
     u64* header = (u64*)darray - DARRAY_FIELD_LENGHT;
     u64 header_size = DARRAY_FIELD_LENGHT * sizeof(u64);
     u64 full_size = header_size + header[DARRAY_CAPACITY] * header[DARRAY_STRIDE];
     tfree(darray, full_size, MEMORY_TAG_DARRAY);
}

u64 darray_field_get(void* darray, u64 field) {
    u64* header = (u64*)darray - DARRAY_FIELD_LENGHT;
    return header[field];
}
void darray_field_set(void* darray, u64 field, u64 value) {
    u64* header = (u64*)darray - DARRAY_FIELD_LENGHT;
    header[field] = value;
}

void* darray_resize(void* darray){
    u64 length = DARRAY_LENGTH(darray);
    u64 stride = DARRAY_STRIDE(darray);
    void* temp = darray_create(DARRAY_RESIZE_FACTOR * DARRAY_CAPACITY(darray), stride);
    copy_memory(temp, darray, length * stride);
    darray_field_set(temp, DARRAY_LENGTH, length);
    darray_destroy(darray);
    return temp;
}

void* darray_push(void* darray, const void* valu_ptr){
    u64 length = DARRAY_LENGTH(darray);
    u64 stride = DARRAY_STRIDE(darray);
    if(length >= DARRAY_CAPACITY(darray)) {
        darray = darray_resize(darray);
    }
    u64 addr = (u64)darray;
    addr += (length * stride);
    copy_memory((void*)addr, valu_ptr, stride);
    darray_field_set(darray, DARRAY_LENGTH, length+1);
    return darray;
}
void darray_pop(void* darray, void* dest) {
    u64 length = DARRAY_LENGTH(darray);
    u64 stride = DARRAY_STRIDE(darray);
    u64 addr = (u64)darray;
    addr += ((length - 1) * stride);
    copy_memory(dest, (void*)addr, stride);
    darray_field_set(darray, DARRAY_LENGTH, length - 1);
}

void* darray_insert(void* darray, u64 index, const void* valu_ptr) {
    u64 length = DARRAY_LENGTH(darray);
    u64 stride = DARRAY_STRIDE(darray);
    if(index >= length) {
        T_ERROR("Out of bound");
        return darray;
    }
    if(length >= DARRAY_CAPACITY(darray)) {
        darray = darray_resize(darray);
    }
    u64 addr = (u64)darray;
    if(index != length - 1) {
        copy_memory((void*)(addr + ((index + 1) * stride)), (void*)(addr + (index * stride)), stride * (length - index));
    }
    copy_memory((void*)(addr + (index * stride)), valu_ptr, stride);
    darray_field_set(darray, DARRAY_LENGTH, length+1);
    return darray;
}

void darray_pop_at(void* darray, u64 index, void* dest) {
    u64 length = DARRAY_LENGTH(darray);
    u64 stride = DARRAY_STRIDE(darray);
    if(index >= length) {
        T_ERROR("Out of bound");
        return;
    }
    u64 addr = (u64)darray;
    copy_memory(dest, ((void*)addr + (index * stride)), stride);

    if(index != length - 1) {
        copy_memory((void*)(addr + (index * stride)), (void*)(addr + ((index + 1) * stride)), stride * (length - index));
    }
    darray_field_set(darray, DARRAY_LENGTH, length-1);
}
