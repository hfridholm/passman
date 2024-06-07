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

  menu_win_input_create((menu_t*) menu, "name", x, 5, w, dbfile, sizeof(dbfile), "Name", false, true, win_input_key_handler);
  
  menu_win_input_create((menu_t*) menu, "email", x, 9, w, database.email, sizeof(database.email), "Email", false, true, win_input_key_handler);

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
