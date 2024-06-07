#ifndef P_MENU_DBS_H
#define P_MENU_DBS_H

#include "p-menu.h"

typedef struct
{
  menu_head_t head;
  char**      dbs_names;
  int         dbs_count;
  char        buffer_search[64];
  char        buffer_new[64];
} menu_dbs_t;

extern menu_dbs_t* menu_dbs_create(char* name, int xmax, int ymax);

extern void        menu_dbs_resize(menu_dbs_t* menu, int xmax, int ymax);

extern void        menu_dbs_free(menu_dbs_t* menu);

#endif // P_MENU_DBS_H
