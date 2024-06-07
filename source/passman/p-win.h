#ifndef P_WIN_H
#define P_WIN_H

typedef enum
{
  WIN_CONFIRM,
  WIN_TEXT,
  WIN_INPUT,
  WIN_LIST,
  WIN_COUNT
} win_type_t;


typedef struct screen_t    screen_t;

typedef struct menu_head_t menu_t;

typedef struct win_head_t win_head_t;

typedef struct win_head_t  win_t;


typedef void key_handler_t(win_head_t*, int);

typedef struct win_head_t
{
  win_type_t     type;
  char*          name;        // Used to identify window
  WINDOW*        window;
  int            ymax;
  int            xmax;
  bool           active;
  menu_t*        menu;
  screen_t*      screen;
  key_handler_t* key_handler;
} win_head_t;

extern win_t* win_head_create(win_type_t type, char* name, int x, int y, int w, int h, bool active, key_handler_t* key_handler);

extern void   win_head_free(win_head_t* win);

extern void   win_head_clean(win_head_t* win);

extern void   win_head_resize(win_head_t* win, int x, int y, int w, int h);


extern void win_free(win_t* win);

extern void win_refresh(win_t* win);


extern void wins_free(win_t** wins, int count);

#include "p-win-input.h"
#include "p-win-list.h"
#include "p-win-confirm.h"
#include "p-win-text.h"

#endif // P_WINDOW_H
