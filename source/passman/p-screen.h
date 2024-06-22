#ifndef P_SCREEN_H
#define P_SCREEN_H

#include "p-win.h"
#include "p-menu.h"

typedef struct screen_t
{
  menu_t** menus;
  int      menu_count;
  int      menu_index;
  win_t**  wins;
  int      win_count;
  bool     running;
  dbase_t* dbase;
} screen_t;

#define TEXT_RED_PAIR 1

extern screen_t* screen_create(void);

extern void      screen_resize(screen_t* screen);

extern void      screen_refresh(screen_t* screen);

extern void      screen_free(screen_t* screen);

extern int       screen_event(screen_t* screen, int key);


extern win_t* active_win_get(screen_t* screen);

extern bool   win_is_active_win(win_t* win);


extern void screen_text_popup(screen_t* screen, char* title, char* text);

#include "p-screen-menu.h"
#include "p-screen-win.h"

#endif // P_SCREEN_H
