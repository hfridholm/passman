#include "../passman.h"

static void menu_dbs_win_search_event_enter(menu_dbs_t* menu, win_input_t* win)
{
  menu_win_focus_set((menu_t*) menu, "dbs");
}

int menu_dbs_win_search_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return 0;

  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return 0;


  menu_dbs_t* menu = (menu_dbs_t*) menu_head;

  win_input_t* win = (win_input_t*) win_head;


  if(win_input_event(win_head, key))
  {
    win_list_t* win_list = menu_win_list_get((menu_t*) menu, "dbs");

    win_list_items_filter(win_list);

    return 1;
  }

  switch(key)
  {
    case KEY_ENTR:
      menu_dbs_win_search_event_enter(menu, win);
      return 2;

    default:
      return 0;
  }
}

