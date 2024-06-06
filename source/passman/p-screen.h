#ifndef P_SCREEN_H
#define P_SCREEN_H

typedef struct
{
  menu_t* menus;
  int     menu_count;
  int     menu_index;
  win_t*  pops;
  int     pop_count;
  int     pop_index;
  bool    running;
} screen_t;

typedef void key_handler_t(screen_t*, win_t*, int);


extern void screen_resize(screen_t* screen);

extern void screen_refresh(screen_t* screen);

#endif // P_SCREEN_H
