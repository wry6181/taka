#include "tmemory.h"

#include "types.h"
#include "arena.h"
#include "logger.h"
#include <stdio.h>

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

void get_memory_usage_str(mem_arena* arena) {
    const u64 gb = 1024 * 1024 * 1024;
    const u64 mb = 1024 * 1024;
    const u64 kb = 1024;

    s8 out_str = STR8_LIT("System memory usage(tagged) \n");
    for(u32 i = 0; i < MEMORY_TAG_MAX_TAGS; ++i) {
        char unit[4] = "XiB";
        f32 amount = 1.0f;
        if(_stats->tagged_allocations[i] >= gb) {
            unit[0] = 'G';
            amount = _stats->tagged_allocations[i] / (f32)gb;
        }
        else if(_stats->tagged_allocations[i] >= gb) {
            unit[0] = 'M';
            amount = _stats->tagged_allocations[i] / (f32)mb;
        }
        else if(_stats->tagged_allocations[i] >= gb) {
            unit[0] = 'K';
            amount = _stats->tagged_allocations[i] / (f32)kb;
        }
        else if(_stats->tagged_allocations[i] >= gb) {
            unit[0] = 'B';
            amount = (f32)_stats->tagged_allocations[i];
        }
        printf("%.*s size: %.2f %.*s", STR8_FMT(out_str), amount, STR8_FMT(STR8_LIT(unit)));

    }


}

//void release_arena_memory(mem_arena* arena, void* block, u64 size, memory_tag tag) {

//     _stats->total_allocated -= size;
//     _stats->tagged_allocations[tag] -= size;

//     arena_pop(arena, size);

// }
