#include "tmemory.h"

#include "types.h"
#include "arena.h"
#include "logger.h"

typedef struct memory_stats memory_stats;
struct memory_stats {
    u64 total_allocated;
    u64 tagged_allocations[MEMORY_TAG_MAX_TAGS];
};

static struct memory_stats* _stats;

void init_memory(mem_arena* arena) {
    _stats = PUSH_STRUCT_ZERO(arena, memory_stats);
}

void* alloc_arena_memory(mem_arena* arena, u64 size, memory_tag tag) {
    if(tag == MEMORY_TAG_UNKNOWN) {
        T_WARNING("Unknown memory allocted, recalssify this allocation");
    }

    _stats->total_allocated += size;
    _stats->tagged_allocations[tag] += size;

    void* block = PUSH_ARRAY_ZERO(arena, void, size);
    return block;
}

void release_arena_memory(mem_arena* arena, void* block, u64 size, memory_tag tag) {

    _stats->total_allocated -= size;
    _stats->tagged_allocations[tag] -= size;

    arena_pop(arena, size)
    
    return block;
}