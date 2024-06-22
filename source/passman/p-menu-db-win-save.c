#include "../passman.h"

/*
 *
 */
static void menu_db_win_save_event_enter(menu_db_t* menu, win_input_t* win)
{
  screen_t* screen = menu->head.screen;

  char* name = menu->dbase->name;

  win_list_t* win_dbs = screen_name_menu_name_win_list_get(screen, "dbs", "dbs");

  if(!win_list_string_item_exists(win_dbs, name))
  {
    win_list_item_add(win_dbs, name, NULL);
  }

  dbase_write(menu->dbase, name, menu->password);

  screen_text_popup(screen, "Save", "Saved dbase");

  win->head.active = false;
}

/*
 *
 */
int menu_db_win_save_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return 0;

  menu_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DB) return 0;


  if(win_input_event(win_head, key)) return 1;


  menu_db_t* menu = (menu_db_t*) menu_head;

  win_input_t* win = (win_input_t*) win_head;

  switch(key)
  {
    case KEY_ENTR:
      menu_db_win_save_event_enter(menu, win);
      return 2;

    case KEY_ESC: case KEY_CTRLZ:
      win_input_buffer_clear(win);

      win->head.active = false;
      return 3;

    default:
      return 0;
  }
}
