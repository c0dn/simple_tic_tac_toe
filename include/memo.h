#ifndef MEMO_H
#define MEMO_H
#include <uthash.h>

typedef struct {
  float btn_width;
  ComponentPadding btn_padding;
  float btn_height;
  float first_button_offset;
  int index;
  int buttons_per_row;
  int screen_height;
  int screen_width;
} ButtonKey;

typedef struct {
  ButtonKey key;
  Rectangle result;
  UT_hash_handle hh;
} ButtonCache;

extern ButtonCache* BUTTON_CACHE;


#endif //MEMO_H
