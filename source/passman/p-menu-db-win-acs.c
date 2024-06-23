#include "../passman.h"

static void menu_db_win_acs_event_open(menu_db_t* menu, win_list_t* win)
{
  char* item = win_list_item_string_get(win);

  if(!item) return;


  dbase_t* dbase = menu->dbase;

  int accnt_index = dbase_name_accnt_index_get(dbase, item);

  if(accnt_index == -1) return;


  screen_t* screen = menu->head.screen;

  if(!screen) return;


  menu_act_t* menu_act = screen_menu_act_get(screen, "act");

  if(!menu_act) return;


  menu_act->accnt = &dbase->accnts[accnt_index];

  menu_act_accnt_fill(menu_act);

  screen_menu_focus_set(menu->head.screen, "act");
}

static void menu_db_win_acs_event_delete(menu_db_t* menu, win_list_t* win)
{
  if(!win_list_item_exists(win)) return;

  menu_win_confirm_answer_set((menu_t*) menu, "delete", false);

  menu_win_focus_set((menu_t*) menu, "delete");
}

static void menu_db_win_acs_event_new(menu_db_t* menu, win_list_t* win)
{
  menu_win_input_buffer_clear((menu_t*) menu, "new");

  menu_win_focus_set((menu_t*) menu, "new");
}

static void menu_db_win_acs_event_rename(menu_db_t* menu, win_list_t* win)
{
  if(!win_list_item_exists(win)) return;

  win_input_t* win_rename = menu_win_input_get((menu_t*) menu, "rename");

  if(!win_rename) return;

  char* item = win_list_item_string_get(win);

  win_input_buffer_paste(win_rename, item);

  menu_win_focus_set((menu_t*) menu, "rename");
}

/*
 *
 */
int menu_db_win_acs_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_LIST) return 0;

  menu_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DB) return 0;


  menu_db_t* menu = (menu_db_t*) menu_head;

  win_list_t* win = (win_list_t*) win_head;


  if(win_list_event(win_head, key))
  {
    // Change item preivew

    return 1;
  }


  switch(key)
  {
    case 'o': case 'O': case KEY_ENTR:
      menu_db_win_acs_event_open(menu, win);
      return 2;

    case 'd': case 'D':
      menu_db_win_acs_event_delete(menu, win);
      return 3;

    case 'n': case 'N':
      menu_db_win_acs_event_new(menu, win);
      return 4;

    case 'r': case 'R':
      menu_db_win_acs_event_rename(menu, win);
      return 5;

    default:
      return 0;
  }
}

