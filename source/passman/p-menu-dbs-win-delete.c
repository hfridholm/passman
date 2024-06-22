#include "../passman.h"

static void menu_dbs_dbase_delete(menu_dbs_t* menu)
{
  win_list_t* win_dbs = menu_win_list_get((menu_t*) menu, "dbs");

  char* item = win_list_hover_item_string_get(win_dbs);


  screen_t* screen = menu->head.screen;


  int status = dbase_file_remove(item);

  if(status == 0)
  {
    win_list_hover_item_delete(win_dbs);
  }
  else screen_text_popup(menu->head.screen, "Error", "Could not delete database");
}

static void menu_dbs_win_delete_event_enter(menu_dbs_t* menu, win_confirm_t* win)
{
  if(win->answer == true)
  {
    menu_dbs_dbase_delete(menu); 
  }

  win->answer = false;

  win->head.active = false;
}

int menu_dbs_win_delete_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_CONFIRM) return 0;

  menu_head_t* menu_head = win_head->menu;

  if(!menu_head || menu_head->type != MENU_DBS) return 0;


  if(win_confirm_event(win_head, key)) return 1;


  menu_dbs_t* menu = (menu_dbs_t*) menu_head;

  win_confirm_t* win = (win_confirm_t*) win_head;

  switch(key)
  {
    case KEY_ESC: case KEY_CTRLZ:
      win->answer = false;

      win->head.active = false;
      return 2;

    case KEY_ENTR:
      menu_dbs_win_delete_event_enter(menu, win);
      return 3;

    default:
      return 0;
  }
}
