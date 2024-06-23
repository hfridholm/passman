#ifndef P_WIN_LIST_H
#define P_WIN_LIST_H

typedef struct
{
  char string [64];
  char preview[64];
} win_list_item_t;

typedef struct
{
  win_head_t        head;
  win_list_item_t*  items;
  int               item_count;
  int               max_item_count;
  win_list_item_t** show_items;
  int               show_item_count;
  int               show_item_index;
  int               scroll;
  char*             buffer_filter;
  bool              order_reverse;
} win_list_t;

extern win_list_t* win_list_create(char* name, bool active, bool tab_ability, int x, int y, int w, int h, int max_item_count, char* buffer_filter, win_event_t* event);

extern void        win_list_free(win_list_t* win);

extern void        win_list_resize(win_list_t* win, int x, int y, int w, int h);

extern void        win_list_refresh(win_list_t* win);

extern int         win_list_event(win_head_t* win_head, int key);

#include "p-win-list-item.h"

#endif // P_WIN_LIST_H
