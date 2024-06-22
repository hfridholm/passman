#include "../passman.h"

static void menu_dbs_win_open_event_enter(menu_dbs_t* menu, win_input_t* win)
{
  char* item = menu_name_win_list_hover_item_string_get((menu_t*) menu, "dbs");

  screen_t* screen = menu->head.screen;

  if(!screen) return;

  refresh();

  printf("dbase: %p\n", menu->dbase);

  getch();

  int status = dbase_read(menu->dbase, item, menu->password);

  if(status == 0)
  {
    screen_name_menu_db_dbase_set(screen, "db", menu->dbase);

    screen_name_menu_focus_set(screen, "db");
  }
  else screen_text_popup(screen, "Error", "Could not open dbase");


  menu_name_win_input_buffer_clear((menu_t*) menu, "open");

  win->head.active = false;
}

void menu_dbs_win_open_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_INPUT) return;

  win_input_event(win_head, key);

  win_input_t* win = (win_input_t*) win_head;


  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return;

  menu_dbs_t* menu = (menu_dbs_t*) menu_head;


  switch(key)
  {
    case KEY_ESC: case KEY_CTRLZ:
      menu_name_win_input_buffer_clear((menu_t*) menu, "open");

      win->head.active = false;
      break;

    case KEY_ENTR:
      menu_dbs_win_open_event_enter(menu, win);
      break;

    default:
      break;
  }
}
