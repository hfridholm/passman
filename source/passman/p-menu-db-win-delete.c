#include "../passman.h"

/*
 *
 */
static void menu_db_accnt_delete(menu_db_t* menu)
{
  win_list_t* win_acs = menu_win_list_get((menu_t*) menu, "acs");

  char* item = win_list_item_string_get(win_acs);

  dbase_name_accnt_delete(menu->dbase, item);

  win_list_item_delete(win_acs);
}

/*
 *
 */
static void menu_db_win_delete_event_enter(menu_db_t* menu, win_confirm_t* win)
{
  if(win->answer == true)
  {
    menu_db_accnt_delete(menu);
  }

  win->answer = false;

  win->head.active = false;
}

int menu_db_win_delete_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_CONFIRM) return 0;

  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DB) return 0;


  if(win_confirm_event(win_head, key)) return 1;


  menu_db_t* menu = (menu_db_t*) menu_head;

  win_confirm_t* win = (win_confirm_t*) win_head;

  switch(key)
  {
    case KEY_ESC: case KEY_CTRLZ:
      win->answer = false;

      win->head.active = false;
      return 2;

    case KEY_ENTR:
      menu_db_win_delete_event_enter(menu, win);
      return 3;

    default:
      return 0;
  }
}
