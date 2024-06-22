#include "../passman.h"

static void menu_db_win_delete_event_enter(menu_db_t* menu, win_confirm_t* win)
{
  win_list_t* win_acs = menu_name_win_list_get((menu_t*) menu, "acs");

  char* item = win_list_hover_item_string_get(win_acs);

  dbase_name_accnt_delete(menu->dbase, item);

  win_list_hover_item_delete(win_acs);

  win->head.active = false;
}

void menu_db_win_delete_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_CONFIRM) return;

  win_confirm_event(win_head, key);

  win_confirm_t* win = (win_confirm_t*) win_head;


  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DB) return;

  menu_db_t* menu = (menu_db_t*) menu_head;

  switch(key)
  {
    case KEY_ENTR:
      menu_db_win_delete_event_enter(menu, win);
      break;

    default:
      break;
  }
}
