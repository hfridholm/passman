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
} screen_t;

extern screen_t* screen_create(void);

extern void      screen_resize(screen_t* screen);

extern void      screen_refresh(screen_t* screen);

extern void      screen_free(screen_t* screen);


extern void screen_key_handler(screen_t* screen, int key);

#include "p-screen-menu.h"
#include "p-screen-win.h"

#endif // P_SCREEN_H
