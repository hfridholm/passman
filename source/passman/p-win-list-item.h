#ifndef P_WIN_LIST_ITEM_H
#define P_WIN_LIST_ITEM_H

#include "p-win-list.h"

extern char* win_list_item_string_get(win_list_t* win);

extern int   win_list_item_string_set(win_list_t* win, const char* string);


extern int  win_list_item_add(win_list_t* win, const char* string, const char* preview);

extern int  win_list_item_delete(win_list_t* win);

extern bool win_list_item_exists(win_list_t* win);


extern bool win_list_string_item_exists(win_list_t* win, const char* string);


extern win_list_t* win_list_items_filter(win_list_t* win);

extern win_list_t* win_list_items_order(win_list_t* win);

extern int         win_list_items_empty(win_list_t* win);

#endif // P_WIN_LIST_ITEM_H
