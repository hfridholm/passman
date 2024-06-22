#include "../passman.h"

static void menu_dbs_win_rename_event_enter(menu_dbs_t* menu, win_input_t* win)
{
  win_list_t* win_list = menu_name_win_list_get((menu_t*) menu, "dbs");

  char* old_name = win_list_hover_item_string_get(win_list);

  char* new_name = win->buffer;

  dbase_file_rename(old_name, new_name);

  win_list_hover_item_set(win_list, new_name, NULL);

  win->head.active = false;
}

void menu_dbs_win_rename_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return;

  win_input_event(win_head, key);

  win_input_t* win = (win_input_t*) win_head;


  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return;

  menu_dbs_t* menu = (menu_dbs_t*) menu_head;

  switch(key)
  {
    case KEY_ENTR:
      menu_dbs_win_rename_event_enter(menu, win);
      break;

    default:
      break;
  }
}

