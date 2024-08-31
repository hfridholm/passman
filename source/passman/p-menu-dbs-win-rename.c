#include "../passman.h"

static void menu_dbs_win_rename_event_enter(menu_dbs_t* menu, win_input_t* win)
{
  if(!menu->head.screen || !win->buffer) return;

  screen_t* screen = menu->head.screen;


  win_list_t* win_dbs = menu_win_list_get((menu_t*) menu, "dbs");

  char* old_name = win_list_item_string_get(win_dbs);

  char* new_name = win->buffer;

  // This following code with Fail popups is the same as for "new". Reuse it!
  if(strlen(new_name) == 0)
  {
    screen_text_popup(screen, "Fail", "The database must have a name");

    return;
  }


  if(win_list_string_item_exists(win_dbs, win->buffer))
  {
    screen_text_popup(screen, "Fail", "A database with the same name already exists");

    return;
  }


  dbase_file_rename(old_name, new_name);


  win_list_item_string_set(win_dbs, new_name);

  win->head.active = false;
}

int menu_dbs_win_rename_event(win_head_t* win_head, int key)
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
      menu_dbs_win_rename_event_enter(menu, win);
      return 3;

    default:
      return 0;
  }
}

