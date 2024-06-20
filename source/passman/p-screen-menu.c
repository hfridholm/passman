#include "../passman.h"

menu_t* screen_menu_get(screen_t* screen)
{
  if(screen->menu_index >= 0 && screen->menu_index < screen->menu_count)
  {
    return screen->menus[screen->menu_index];
  }
  return NULL;
}

void screen_name_menu_focus_set(screen_t* screen, char* menu_name)
{
  for(size_t index = 0; index < screen->menu_count; index++)
  {
    menu_t* menu = screen->menus[index];

    if(strcmp(menu->name, menu_name) != 0) continue;

    screen->menu_index = index;

    break;
  }
}

void screen_menu_add(screen_t* screen, menu_t* menu)
{
  screen->menus = realloc(screen->menus, sizeof(menu_t*) * (screen->menu_count + 1));

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

menu_t* screen_name_menu_get(screen_t* screen, char* menu_name)
{
  if(!screen || !screen->menus) return NULL;

  for(size_t index = 0; index < screen->menu_count; index++)
  {
    menu_t* menu = screen->menus[index];

    if(!menu) continue;

    if(strcmp(menu->name, menu_name) == 0) return menu;
  }

  return NULL;
}

menu_db_t* screen_name_menu_db_get(screen_t* screen, char* menu_name)
{
  menu_t* menu = screen_name_menu_get(screen, menu_name);

  if(!menu || menu->type != MENU_DB) return NULL;

  return (menu_db_t*) menu;
}

void screen_name_menu_db_dbase_set(screen_t* screen, char* menu_name, dbase_t* dbase)
{
  menu_db_t* menu = screen_name_menu_db_get(screen, menu_name);

  menu_db_dbase_set(menu, dbase);
}
