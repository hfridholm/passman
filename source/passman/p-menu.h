#ifndef P_MENU_H
#define P_MENU_H

typedef enum
{
  MENU_DB,
  MENU_DBS,
  MENU_PSW,
  MENU_COUNT
} menu_type_t;

typedef struct
{
  menu_type_t type;
  char*       name;      // Used to identify menu
  win_t**     wins;
  int         win_count;
  int         win_index;
  win_t**     pops;
  int         pop_count;
  int         pop_index;
} menu_head_t;

typedef menu_t menu_head_t;

extern void menu_resize(menu_t* menu, int xmax, int ymax);

extern void menu_refresh(menu_t* menu);

#endif // P_MENU_H
