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


typedef struct {
  float width_percentage;
  float height_percentage;
  int screen_height;
  int screen_width;
} BoxKey;


typedef struct {
  BoxKey key;
  BoxDimensions result;
  UT_hash_handle hh;
} BoxCache;


typedef struct {
  char message[512];  // Adjust size as needed
  int font_size;
  float ref_x;
  float ref_y;
  float ref_width;
  float ref_height;
} TextKey;

typedef struct {
  TextKey key;
  Coords result;
  UT_hash_handle hh;
} TextCache;


typedef struct {
  char message[256];
  int font_size;
  float ref_x;
  float ref_y;
  float ref_width;
  float ref_height;
  float vertical_offset_percent;
  float horizontal_offset_percent;
} OffsetTextKey;


typedef struct {
  OffsetTextKey key;
  Coords result;
  UT_hash_handle hh;
} OffsetTextCache;


extern ButtonCache* BUTTON_CACHE;
extern BoxCache* BOX_CACHE;
extern TextCache* TEXT_CACHE;
extern OffsetTextCache* OFFSET_TEXT_CACHE;


#endif //MEMO_H
