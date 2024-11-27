#include "memo.h"

MemoCache* init_memo_cache(void)
{
    MemoCache* cache = malloc(sizeof(MemoCache));
    if (cache)
    {
        cache->box_cache = NULL;
    }
    return cache;
}

/**
 * Deallocates memory for a memoization cache
 *
 * @param cache Pointer to MemoCache structure to be freed
 */
void cleanup_memo_cache(MemoCache* cache)
{
    if (!cache) return;

    BoxCache *current_box, *tmp_box;
    HASH_ITER(hh, cache->box_cache, current_box, tmp_box)
    {
        HASH_DEL(cache->box_cache, current_box);
        free(current_box);
    }

    free(cache);

    cache = NULL;
}
