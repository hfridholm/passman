#ifndef P_MENU_DB_H
#define P_MENU_DB_H

#include "p-menu.h"

typedef struct
{
  menu_head_t head;
} menu_db_t;

extern menu_db_t* menu_db_create(char* name, int xmax, int ymax);

extern void       menu_db_resize(menu_db_t* menu, int xmax, int ymax);

extern void       menu_db_free(menu_db_t* menu);

#endif // P_MENU_DB_H
