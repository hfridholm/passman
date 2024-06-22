#include "../passman.h"

static void menu_db_win_acs_event_open(menu_db_t* menu, win_list_t* win)
{
  screen_t* screen = menu->head.screen;

  screen_name_menu_focus_set(screen, "act");
}

static void menu_db_win_acs_event_delete(menu_db_t* menu, win_list_t* win)
{
  menu_name_win_confirm_answer_set((menu_t*) menu, "delete", false);

  menu_name_win_focus_set((menu_t*) menu, "delete");
}

static void menu_db_win_acs_event_new(menu_db_t* menu, win_list_t* win)
{
  menu_name_win_input_buffer_clear((menu_t*) menu, "new");

  menu_name_win_focus_set((menu_t*) menu, "new");
}

static void menu_db_win_acs_event_rename(menu_db_t* menu, win_list_t* win)
{
  win_input_t* win_rename = menu_name_win_input_get((menu_t*) menu, "rename");

  if(!win_rename) return;

  char* item = win_list_hover_item_string_get(win);

  if(!item) return;

  win_input_buffer_paste(win_rename, item);

  menu_name_win_focus_set((menu_t*) menu, "rename");
}

void menu_db_win_acs_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_LIST) return;

  win_list_t* win = (win_list_t*) win_head;


  int last_item_index = win->item_index;


  win_list_event(win_head, key);


  char* string = win_list_hover_item_string_get(win);

  char banner[128];

  sprintf(banner, "%s - hover", string);

  win_list_hover_item_banner_set(win, banner);

  char* last_string = win_list_item_string_get(win, last_item_index);

  win_list_item_banner_set(win, last_item_index, last_string);


  menu_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DB) return;

  menu_db_t* menu = (menu_db_t*) menu_head;

  switch(key)
  {
    case 'o': case KEY_ENTR:
      menu_db_win_acs_event_open(menu, win);
      break;

    case 'd':
      menu_db_win_acs_event_delete(menu, win);
      break;

    case 'n':
      menu_db_win_acs_event_new(menu, win);
      break;

    case 'r':
      menu_db_win_acs_event_rename(menu, win);
      break;

    default:
      break;
  }
}

