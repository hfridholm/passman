#include "../passman.h"

menu_t* screen_menu_get(screen_t* screen)
{
  if(screen->menu_index >= 0 && screen->menu_index < screen->menu_count)
  {
    return screen->menus[screen->menu_index];
  }
  return NULL;
}

void screen_menu_add(screen_t* screen, menu_t* menu)
{
  if(screen->menus == NULL)
  {
    screen->menus = malloc(sizeof(menu_t*));
  }
  else screen->menus = realloc(screen->menus, sizeof(menu_t*) * (screen->menu_count + 1));

  screen->menus[screen->menu_count++] = menu;

  menu->screen = screen;

  for(int index = 0; index < menu->win_count; index++)
  {
    menu->wins[index]->screen = screen;
  }
}

void screen_menu_dbs_create(screen_t* screen, char* name, int xmax, int ymax)
{
  menu_dbs_t* menu = menu_dbs_create(name, xmax, ymax);

  screen_menu_add(screen, (menu_t*) menu);
}

void screen_menu_db_create(screen_t* screen, char* name, int xmax, int ymax)
{
  menu_db_t* menu = menu_db_create(name, xmax, ymax);

  screen_menu_add(screen, (menu_t*) menu);
}

void screen_menu_psw_create(screen_t* screen, char* name, int xmax, int ymax)
{
  menu_psw_t* menu = menu_psw_create(name, xmax, ymax);

  screen_menu_add(screen, (menu_t*) menu);
}
