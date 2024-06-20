#ifndef P_MENU_H
#define P_MENU_H

#include "p-win.h"

typedef enum
{
  MENU_DB,
  MENU_DBS,
  MENU_PSW,
  MENU_COUNT
} menu_type_t;

typedef struct menu_head_t menu_head_t;

typedef struct menu_head_t menu_t;


typedef void menu_event_t(menu_head_t*, int);

typedef struct menu_head_t
{
  menu_type_t   type;
  char*         name;      // Used to identify menu
  win_t**       wins;
  int           win_count;
  screen_t*     screen;
  menu_event_t* event; 
} menu_head_t;


extern menu_head_t menu_head_create(menu_type_t type, char* name);

extern void        menu_head_free(menu_head_t menu);


extern void menu_resize(menu_t* menu, int xmax, int ymax);

extern void menu_refresh(menu_t* menu);

extern void menu_event(menu_t* menu, int key);


extern void menus_free(menu_t** menus, int count);

#include "p-menu-win.h"

#include "p-menu-db.h"
#include "p-menu-dbs.h"
#include "p-menu-psw.h"

#endif // P_MENU_H
