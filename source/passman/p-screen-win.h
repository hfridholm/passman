#ifndef P_SCREEN_WIN_H
#define P_SCREEN_WIN_H

extern void screen_win_add(screen_t* screen, win_t* win);

extern win_t* screen_active_win_get(screen_t* screen);

extern void screen_win_focus_set(screen_t* screen, char* win_name);


extern win_text_t*    screen_win_text_get(screen_t* screen, char* win_name);

extern win_confirm_t* screen_win_confirm_get(screen_t* screen, char* win_name);


extern void screen_win_input_create(screen_t* screen, char* name, bool ability, bool tab_ability, int x, int y, int w, char* buffer, size_t size, char* title, bool secret, win_event_t* event);

extern void screen_win_text_create(screen_t* screen, char* name, bool ability, bool tab_ability, int x, int y, int w, int h, char* title, char* text, win_event_t* event);

extern void screen_win_list_create(screen_t* screen, char* name, bool ability, bool tab_ability, int x, int y, int w, int h, int max_count, win_event_t* event);

extern void screen_win_confirm_create(screen_t* screen, char* name, bool ability, bool tab_ability, int x, int y, int w, char* prompt, char* text_yes, char* text_no, win_event_t* event);


extern void screen_win_confirm_resize(screen_t* screen, char* name, int x, int y, int w);

extern void screen_win_text_resize(screen_t* screen, char* name, int x, int y, int w, int h);

#endif // P_SCREEN_WIN_H
