#include "../passman.h"

/*
 * Note: Implement return status (fail or success i alla fall), for all event_enter functions
 */
static void menu_db_win_new_event_enter(menu_db_t* menu, win_input_t* win)
{
  if(!menu->head.screen || !win->buffer || !menu->dbase) return;

  screen_t* screen = menu->head.screen;


  if(strlen(win->buffer) == 0)
  {
    screen_text_popup(screen, "Fail", "The account must have a name");

    return;
  }


  win_list_t* win_acs = menu_win_list_get((menu_t*) menu, "acs");

  if(win_list_string_item_exists(win_acs, win->buffer))
  {
    screen_text_popup(screen, "Fail", "An account with the same name already exists");

    return;
  }

  dbase_t* dbase = menu->dbase;

  if(dbase->accnt_count >= DBASE_ACCNTS_SIZE) return;

  // 2. Add new account and add it to the account list
  accnt_t* accnt = &dbase->accnts[dbase->accnt_count++];

  strncpy(accnt->name, win->buffer, sizeof(accnt->name));

  win_list_item_add(win_acs, accnt->name, NULL);

  // 3. Fill out the fields in account menu and switch to it
  screen_menu_act_accnt_fill(screen, "act", accnt);

  screen_menu_focus_set(screen, "act");

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
