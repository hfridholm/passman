#ifndef P_MENU_POP_H
#define P_MENU_POP_H

extern win_t* menu_pop_get(menu_t* menu);


extern int menu_pop_input_resize(menu_t* menu, const char* pop_name, int x, int y, int w);

extern int menu_pop_list_resize(menu_t* menu, const char* pop_name, int x, int y, int w, int h);

extern int menu_pop_confirm_resize(menu_t* menu, const char* pop_name, int x, int y, int w);


extern void menu_pop_input_create(menu_t* menu, char* name, int x, int y, int w, char* buffer, size_t size, char* title, bool secret, bool active, key_handler_t* key_handler);

extern void menu_pop_confirm_create(menu_t* menu, char* name, int x, int y, int w, char* prompt, char* text_yes, char* text_no, bool active, key_handler_t* key_handler);

#endif // P_MENU_POP_H
