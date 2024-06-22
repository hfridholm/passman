#include "../passman.h"

static void menu_dbs_win_new_event_enter(menu_dbs_t* menu, win_input_t* win)
{
  strcpy(menu->dbase->name, win->buffer);


  win_list_t* win_list = menu_name_win_list_get((menu_t*) menu, "dbs");

  win_list_item_add(win_list, win->buffer, NULL);


  screen_t* screen = menu->head.screen;

  if(!screen) return;

  screen_name_menu_db_dbase_set(screen, "db", screen->dbase);

  screen_name_menu_focus_set(screen, "db");


  win->head.active = false;
}

int menu_dbs_win_new_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return 0;

  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return 0;


  if(win_input_event(win_head, key)) return 1;


  menu_dbs_t* menu = (menu_dbs_t*) menu_head;

  win_input_t* win = (win_input_t*) win_head;

  switch(key)
  {
    case KEY_ENTR:
      menu_dbs_win_new_event_enter(menu, win);
      return 2;

    default:
      return 0;
  }
}

