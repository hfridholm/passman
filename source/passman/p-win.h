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

typedef struct win_head_t win_t;


typedef int win_event_t(win_head_t*, int);

typedef struct win_head_t
{
  win_type_t   type;
  char*        name;        // Used to identify window
  WINDOW*      window;
  int          ymax;
  int          xmax;
  bool         active;
  bool         tab_ability;
  menu_t*      menu;
  screen_t*    screen;
  win_event_t* event;
} win_head_t;

extern win_head_t win_head_create(win_type_t type, char* name, bool active, bool tab_ability, int x, int y, int w, int h, win_event_t* event);

extern void        win_head_free(win_head_t win);

extern void        win_head_clean(win_head_t* win);

extern void        win_head_resize(win_head_t* win, int x, int y, int w, int h);


extern void wins_free(win_t** wins, int count);

extern void wins_refresh(win_t** wins, int count);

extern void   wins_rotate(win_t** wins, int win_count, int turns);


extern win_t* wins_active_win_get(win_t** wins, int count);

extern void   wins_name_win_focus_set(win_t** wins, int count, char* name);

extern win_t* wins_name_win_get(win_t** wins, int count, char* name);


extern void win_border_print(win_t* win);

#include "p-win-input.h"
#include "p-win-list.h"
#include "p-win-confirm.h"
#include "p-win-text.h"

#endif // P_WIN_H
