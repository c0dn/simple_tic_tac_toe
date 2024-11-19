#include "memo.h"

MemoCache* init_memo_cache(void) {
    MemoCache* cache = malloc(sizeof(MemoCache));
    if (cache) {
        cache->button_cache = NULL;
        cache->box_cache = NULL;
    }
    return cache;
}

void cleanup_memo_cache(MemoCache* cache) {
    if (!cache) return;

    ButtonCache *current_btn, *tmp_btn;
    HASH_ITER(hh, cache->button_cache, current_btn, tmp_btn) {
        HASH_DEL(cache->button_cache, current_btn);
        free(current_btn);
    }

    BoxCache *current_box, *tmp_box;
    HASH_ITER(hh, cache->box_cache, current_box, tmp_box) {
        HASH_DEL(cache->box_cache, current_box);
        free(current_box);
    }

    free(cache);

    cache = NULL;
}