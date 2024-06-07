#ifndef P_SCREEN_POP_H
#define P_SCREEN_POP_H

extern win_t* screen_pop_get(screen_t* screen);

extern void screen_pop_add(screen_t* screen, win_t* pop);


extern void screen_pop_confirm_create(screen_t* screen, char* name, int x, int y, int w, char* prompt, char* text_yes, char* text_no, bool active, key_handler_t* key_handler);

extern void screen_pop_text_create(screen_t* screen, char* name, int x, int y, int w, int h, char* title, char* text, bool active, key_handler_t* key_handler);


extern int screen_pop_confirm_resize(screen_t* screen, char* name, int x, int y, int w);

extern int screen_pop_text_resize(screen_t* screen, char* name, int x, int y, int w, int h);

#endif // P_SCREEN_POP_H
