#include "../passman.h"

menu_head_t menu_head_create(menu_type_t type, char* name, menu_event_t* event)
{
  menu_head_t menu;

  menu.type = type;
  menu.name = name;

  menu.wins = NULL;
  menu.win_count = 0;

  menu.screen = NULL;

  menu.event = event;

  return menu;
}

void menu_refresh(menu_t* menu)
{
  wins_refresh(menu->wins, menu->win_count);

  /*
  wmove(stdscr, 0, 0);

  for(int index = 0; index < menu->win_count; index++)
  {
    win_t* win = menu->wins[index];

    printw("%02d [%c] %s\n", index, 
      (win->active) ? 'X' : ' ',
      win->name);
  }
  */
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

    case MENU_ACT:
      menu_act_resize((menu_act_t*) menu, xmax, ymax);
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

    case MENU_ACT:
      menu_act_free((menu_act_t*) menu);
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
}

int menu_win_enter_tab(menu_head_t* menu)
{
  win_t* win = menu_active_win_get(menu);

  if(!win || win->type != WIN_INPUT) return 0;

  menu_win_tab(menu, false);

  return 1;
}

int menu_tab_event(menu_head_t* menu, int key)
{
  switch(key)
  {
    case KEY_TAB:
      menu_win_tab(menu, false);
      return 1;
    
    case KEY_BTAB:
      menu_win_tab(menu, true);
      return 2;

    case KEY_ENTR:
      return menu_win_enter_tab(menu) ? 3 : 0;

    default:
      return 0;
  }
}

/*
 *
 */
int menu_event(menu_t* menu, int key)
{
  win_t* win = menu_active_win_get(menu);

  // 1. Interact with window
  if(win && win->event && win->event(win, key)) return 1;

  // 2. Special menu commands
  else if(menu && menu->event && menu->event(menu, key)) return 2;

  // 3. Tabbing between windows
  else if(win && win->tab_ability && menu_tab_event(menu, key)) return 3;

  else return 0;
}
