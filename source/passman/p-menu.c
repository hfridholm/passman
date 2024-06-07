#include "../passman.h"

menu_head_t menu_head_create(menu_type_t type, char* name)
{
  menu_head_t menu;

  menu.type = type;
  menu.name = name;

  menu.win_count = 0;
  menu.win_index = -1;

  menu.pop_count = 0;
  menu.pop_index = -1;

  return menu;
}

void menu_refresh(menu_t* menu)
{
  for(int index = 0; index < menu->win_count; index++)
  {
    if(index == menu->win_index) continue;

    win_refresh(menu->wins[index]);
  }

  win_refresh(menu->wins[menu->win_index]);
}

void menu_resize(menu_t* menu, int xmax, int ymax)
{
  switch(menu->type)
  {
    case MENU_DB:
      menu_db_resize((menu_db_t*) menu, xmax, ymax);
      break;

    case MENU_DBS:
      menu_dbs_resize((menu_dbs_t*) menu, xmax, ymax);
      break;

    case MENU_PSW:
      menu_psw_resize((menu_psw_t*) menu, xmax, ymax);
      break;

    default:
      break;
  }
}

void menu_free(menu_t* menu)
{
  switch(menu->type)
  {
    case MENU_DB:
      menu_db_free((menu_db_t*) menu);
      break;

    case MENU_DBS:
      menu_dbs_free((menu_dbs_t*) menu);
      break;

    case MENU_PSW:
      menu_psw_free((menu_psw_t*) menu);
      break;

    default:
      break;
  }
}

void menus_free(menu_t** menus, int count)
{
  for(int index = 0; index < count; index++)
  {
    menu_free(menus[index]);
  }
}

void menu_head_free(menu_head_t menu)
{
  if(menu.wins != NULL)
  {
    wins_free(menu.wins, menu.win_count);

    free(menu.wins);
  }

  if(menu.pops != NULL)
  {
    wins_free(menu.pops, menu.pop_count);

    free(menu.pops);
  }
}

void menu_head_key_handler(menu_head_t* menu, int key)
{
  switch(key)
  {
    case KEY_TAB:
      menu_win_tab(menu, false);
      break;
    
    case KEY_BTAB:
      menu_win_tab(menu, true);
      break;

    default:
      break;
  }
}

void menu_key_handler(menu_t* menu, int key)
{
  win_t* pop = menu_pop_get(menu);
  win_t* win = menu_win_get(menu);

  if(pop != NULL && pop->active && pop->key_handler)
  {
    pop->key_handler(pop, key);
  }
  else if(win != NULL && win->active && win->key_handler)
  {
    menu_head_key_handler(menu, key);

    win->key_handler(win, key);
  }
}
