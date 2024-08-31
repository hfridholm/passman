#include "../passman.h"

/*
 * EXPECT:
 * - menu and win are allocated
 */
static void menu_db_win_save_event_enter(menu_db_t* menu, win_input_t* win)
{
  if(!menu->head.screen || !menu->dbase) return;

  screen_t* screen = menu->head.screen;

  if(strlen(menu->password) == 0)
  {
    screen_text_popup(screen, "Fail", "You must input a password");

    return;
  }

  dbase_save(menu->dbase, menu->dbase->name, menu->password);

  screen_text_popup(screen, "Info", "Saved database");

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
