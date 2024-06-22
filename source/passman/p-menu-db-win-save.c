#include "../passman.h"

/*
 *
 */
static void menu_db_win_save_event_enter(menu_db_t* menu, win_input_t* win)
{
  screen_t* screen = menu->head.screen;

  if(!screen) return;

  dbase_write(menu->dbase, menu->dbase->name, menu->password);

  screen_text_popup(screen, "Save", "Saved dbase");

  win->head.active = false;
}

/*
 *
 */
void menu_db_win_save_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return;

  win_input_event(win_head, key);

  win_input_t* win = (win_input_t*) win_head;


  menu_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DB) return;

  menu_db_t* menu = (menu_db_t*) menu_head;

  switch(key)
  {
    case KEY_ENTR:
      menu_db_win_save_event_enter(menu, win);
      break;

    default:
      break;
  }
}

