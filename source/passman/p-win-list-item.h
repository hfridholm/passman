#ifndef P_WIN_LIST_ITEM_H
#define P_WIN_LIST_ITEM_H

#include "p-win-list.h"

extern char* win_list_item_string_get(win_list_t* win, int index);

extern void  win_list_item_string_set(win_list_t* win, int index, const char* string);


extern char* win_list_item_banner_get(win_list_t* win, int index);

extern void  win_list_item_banner_set(win_list_t* win, int index, const char* banner);


extern void  win_list_hover_item_string_set(win_list_t* win, const char* string);

extern char* win_list_hover_item_string_get(win_list_t* win);


extern char* win_list_hover_item_banner_get(win_list_t* win);

extern void  win_list_hover_item_banner_set(win_list_t* win, const char* banner);


extern void win_list_hover_item_set(win_list_t* win, const char* string, const char* banner);


extern void  win_list_item_add(win_list_t* win, const char* string, const char* banner);

extern void  win_list_item_delete(win_list_t* win, int item_index);


extern void win_list_hover_item_delete(win_list_t* win);

extern bool win_list_hover_item_exists(win_list_t* win);

extern bool win_list_string_item_exists(win_list_t* win, const char* string);

#endif // P_WIN_LIST_ITEM_H
