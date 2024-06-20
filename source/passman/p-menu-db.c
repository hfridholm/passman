#include "../passman.h"

/*
 *
 */
void menu_db_resize(menu_db_t* menu, int xmax, int ymax)
{
  int x = xmax / 2;
  int w = xmax - 12;

  menu_win_input_resize((menu_t*) menu, "name", x, 5, w);

  menu_win_input_resize((menu_t*) menu, "email", x, 9, w);
}

/*
 *
 */
menu_db_t* menu_db_create(char* name, int xmax, int ymax)
{
  menu_db_t* menu = malloc(sizeof(menu_db_t));

  menu->head = menu_head_create(MENU_DB, name);

  int x = xmax / 2;
  int w = xmax - 12;

  menu_win_input_create((menu_t*) menu, "name", true, true,
    x, 5, w, NULL, 0, "Name", false, win_input_event);
  
  menu_win_input_create((menu_t*) menu, "email", true, true,
    x, 9, w, NULL, 0, "Email", false, win_input_event);

  return menu;
}

/*
 *
 */
void menu_db_free(menu_db_t* menu)
{
  if(menu == NULL) return;

  menu_head_free(menu->head);

  free(menu);
}

void menu_db_dbase_set(menu_db_t* menu, dbase_t* dbase)
{
  menu_name_win_input_buffer_set((menu_t*) menu, "name", dbase->name, sizeof(dbase->name));

  menu_name_win_input_buffer_set((menu_t*) menu, "email", dbase->email, sizeof(dbase->email));
}
