#include "../passman.h"

static const dbase_t DBASE_EMPTY;

/*
 * EXPECT:
 * - menu and win are allocated
 */
static void menu_dbs_win_new_event_enter(menu_dbs_t* menu, win_input_t* win)
{
  if(!menu->head.screen || !win->buffer) return;

  screen_t* screen = menu->head.screen;

  if(!screen || !screen->dbase) return;


  if(strlen(win->buffer) == 0)
  {
    screen_text_popup(screen, "Fail", "The database must have a name");

    return;
  }


  win_list_t* win_dbs = menu_win_list_get((menu_t*) menu, "dbs");


  if(win_list_string_item_exists(win_dbs, win->buffer))
  {
    screen_text_popup(screen, "Fail", "A database with the same name already exists");

    return;
  }

  // 1. Create new database and allocate new name
  dbase_t* dbase = screen->dbase;

  *dbase = DBASE_EMPTY;

  strncpy(dbase->name, win->buffer, sizeof(dbase->name));

  win_list_item_add(win_dbs, dbase->name, NULL);

  // 2. Open the database menu with the newly created database
  screen_menu_db_dbase_fill(screen, "db", screen->dbase);

  screen_menu_focus_set(screen, "db");

  win->head.active = false;
}

int menu_dbs_win_new_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return 0;

  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return 0;


  if(win_input_event(win_head, key)) return 1;


  menu_dbs_t* menu = (menu_dbs_t*) menu_head;

  win_input_t* win = (win_input_t*) win_head;

  switch(key)
  {
    case KEY_ESC: case KEY_CTRLZ:
      win_input_buffer_clear(win);

      win->head.active = false;
      return 2;

    case KEY_ENTR:
      menu_dbs_win_new_event_enter(menu, win);
      return 3;

    default:
      return 0;
  }
}

