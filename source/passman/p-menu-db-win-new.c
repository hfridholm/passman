#include "../passman.h"

/*
 *
 */
static void menu_db_win_new_event_enter(menu_db_t* menu, win_input_t* win)
{
  dbase_t* dbase = menu->dbase;

  if(!dbase) return;

  if(dbase->accnt_count >= DBASE_ACCNTS_SIZE) return;

  accnt_t* accnt = &dbase->accnts[dbase->accnt_count];

  strncpy(accnt->name, win->buffer, sizeof(accnt->name));

  dbase->accnt_count++;

  menu_win_list_item_add((menu_t*) menu, "acs", accnt->name, NULL);

  win_input_buffer_clear(win);

  win->head.active = false;
}

/*
 *
 */
int menu_db_win_new_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return 0;

  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DB) return 0;


  if(win_input_event(win_head, key)) return 1;


  menu_db_t* menu = (menu_db_t*) menu_head;

  win_input_t* win = (win_input_t*) win_head;

  switch(key)
  {
    case KEY_ESC: case KEY_CTRLZ:
      win_input_buffer_clear(win);

      win_head->active = false;
      return 2;

    case KEY_ENTR:
      menu_db_win_new_event_enter(menu, win);
      return 3;

    default:
      return 0;
  }
}
