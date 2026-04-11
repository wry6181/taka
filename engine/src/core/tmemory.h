#pragma once

#include "arena.h"
#include "types.h"

typedef enum memory_tag memory_tag;
enum memory_tag {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_DICT,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_BSD,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APP,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXURE,
    MEMORY_TAG_MATERIAL_INST,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_TRANSFORM,
    MEMORY_TAG_ENGINE,
    MEMORY_TAG_ENITIY,
    MEMORY_TAG_ENTITY_NODE,
    MEMORY_TAG_SCENE,

    MEMORY_TAG_MAX_TAGS,
};

void init_memory(mem_arena* arena);

void* alloc_arena_memory(mem_arena* arena, u64 size, memory_tag tag);

void get_memory_usage_str(mem_arena* arena);

//void release_arena_memory(mem_arena* arena, void* block, u64 size, memory_tag tag);
