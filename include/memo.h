#ifndef MEMO_H
#define MEMO_H

#include "common.h"

MemoCache* init_memo_cache(void);

void cleanup_memo_cache(MemoCache* cache);

#endif //MEMO_H