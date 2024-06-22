#include "../passman.h"

/*
 *
 */
static void menu_db_win_rename_event_enter(menu_db_t* menu, win_input_t* win)
{
  win_list_t* win_list = menu_name_win_list_get((menu_t*) menu, "acs");

  char* new_name = win->buffer;

  win_list_hover_item_set(win_list, new_name, NULL);

  win_input_buffer_clear(win);

  win->head.active = false;
}

/*
 *
 */
void menu_db_win_rename_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return;

  win_input_event(win_head, key);

  win_input_t* win = (win_input_t*) win_head;


  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DB) return;

  menu_db_t* menu = (menu_db_t*) menu_head;

  switch(key)
  {
    case KEY_ESC: case KEY_CTRLZ:
      win_input_buffer_clear(win);

      win->head.active = false;
      break;

    case KEY_ENTR:
      menu_db_win_rename_event_enter(menu, win);
      break;

    default:
      break;
  }
}

