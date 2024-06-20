#ifndef P_SCREEN_WIN_H
#define P_SCREEN_WIN_H

extern void screen_win_add(screen_t* screen, win_t* win);

extern win_t* screen_active_win_get(screen_t* screen);


extern void screen_win_input_create(screen_t* screen, char* name, bool ability, bool tab_ability, int x, int y, int w, char* buffer, size_t size, char* title, bool secret, key_handler_t* key_handler);

extern void screen_win_text_create(screen_t* screen, char* name, bool ability, bool tab_ability, int x, int y, int w, int h, char* title, char* text, key_handler_t* key_handler);

extern void screen_win_list_create(screen_t* screen, char* name, bool ability, bool tab_ability, int x, int y, int w, int h, key_handler_t* key_handler);

extern void screen_win_confirm_create(screen_t* screen, char* name, bool ability, bool tab_ability, int x, int y, int w, char* prompt, char* text_yes, char* text_no, key_handler_t* key_handler);


extern int screen_win_confirm_resize(screen_t* screen, char* name, int x, int y, int w);

extern int screen_win_text_resize(screen_t* screen, char* name, int x, int y, int w, int h);

#endif // P_SCREEN_WIN_H
