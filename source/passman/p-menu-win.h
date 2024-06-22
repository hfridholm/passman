#ifndef P_MENU_WIN_H
#define P_MENU_WIN_H

extern void   menu_win_tab(menu_t* menu, bool reverse);

extern win_t* menu_active_win_get(menu_t* menu);

extern void   menu_name_win_focus_set(menu_t* menu, char* win_name);


extern win_t*         menu_name_win_get(menu_t* menu, char* win_name);

extern win_list_t*    menu_name_win_list_get(menu_t* menu, char* win_name);

extern win_confirm_t* menu_name_win_confirm_get(menu_t* menu, char* win_name);

extern win_text_t*    menu_name_win_text_get(menu_t* menu, char* win_name);

extern win_input_t*   menu_name_win_input_get(menu_t* menu, char* win_name);


extern void menu_win_input_resize(menu_t* menu, char* win_name, int x, int y, int w);

extern void menu_win_list_resize(menu_t* menu, char* win_name, int x, int y, int w, int h);

extern void menu_win_confirm_resize(menu_t* menu, char* win_name, int x, int y, int w);


extern void menu_win_input_create(menu_t* menu, char* name, bool ability, bool tab_ability, int x, int y, int w, char* buffer, size_t size, char* title, bool secret, win_event_t* event);

extern void menu_win_text_create(menu_t* menu, char* name, bool ability, bool tab_ability, int x, int y, int w, int h, char* title, char* text, win_event_t* event);

extern void menu_win_list_create(menu_t* menu, char* name, bool ability, bool tab_ability, int x, int y, int w, int h, int max_count, win_event_t* event);

extern void menu_win_confirm_create(menu_t* menu, char* name, bool ability, bool tab_ability, int x, int y, int w, char* prompt, char* text_yes, char* text_no, win_event_t* event);


extern void menu_win_add(menu_t* menu, win_t* win);


extern void menu_name_win_input_buffer_set(menu_t* menu, char* win_name, char* buffer, size_t size);

extern void menu_name_win_input_buffer_clear(menu_t* menu, char* win_name);

extern void menu_name_win_input_buffer_update(menu_t* menu, char* win_name);


extern char* menu_name_win_list_hover_item_string_get(menu_t* menu, char* win_name);

extern void  menu_name_win_list_item_add(menu_t* menu, char* win_name, const char* string, const char* banner);


extern void menu_name_win_confirm_answer_set(menu_t* menu, char* win_name, bool answer);

extern bool menu_name_win_confirm_answer_get(menu_t* menu, char* win_name);

#endif // P_MENU_WIN_H
