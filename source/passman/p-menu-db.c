#include "../passman.h"

/*
 *
 */
void menu_db_resize(menu_db_t* menu, int xmax, int ymax)
{
  int x = xmax / 2;
  int w = xmax - 12;

  win_input_resize(menu->wins[0], x, 5, w);

  win_input_resize(menu->wins[1], x, 9, w);
}

/*
 *
 */
void menu_db_create(int xmax, int ymax)
{
  menu_db_t* menu = malloc(sizeof(menu_db_t));

  menu->type = MENU_DB;

  menu->win_count = 2;

  menu->wins = malloc(sizeof(win_t*) * menu->win_count);

  int x = xmax / 2;
  int w = xmax - 12;

  menu->wins[0] = win_input_create(x, 5, w, dbfile, sizeof(dbfile), "Name", false, true);
  
  menu->wins[1] = win_input_create(x, 9, w, database.email, sizeof(database.email), "Email", false, true);
}

/*
 *
 */
void menu_db_free(menu_db_t* menu)
{
  if(menu == NULL) return;

  menu_head_free(menu);

  free(menu);
}
