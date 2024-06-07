#ifndef P_MENU_WIN_H
#define P_MENU_WIN_H

extern win_t* menu_win_get(menu_t* menu);

extern void   menu_win_tab(menu_t* menu, bool reverse);


extern int menu_win_input_resize(menu_t* menu, const char* win_name, int x, int y, int w);

extern int menu_win_list_resize(menu_t* menu, const char* win_name, int x, int y, int w, int h);

extern int menu_win_confirm_resize(menu_t* menu, const char* win_name, int x, int y, int w);


extern void menu_win_input_create(menu_t* menu, char* name, int x, int y, int w, char* buffer, size_t size, char* title, bool secret, bool active, key_handler_t* key_handler);

extern void menu_win_list_create(menu_t* menu, char* name, int x, int y, int w, int h, char** items, int count, bool active, key_handler_t* key_handler);

#endif // P_MENU_WIN_H
