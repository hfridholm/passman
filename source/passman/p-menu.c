#include "../passman.h"

void menu_refresh(menu_t* menu)
{
  for(int index = 0; index < menu->win_count; index++)
  {
    if(index == menu->win_index) continue;

    win_refresh(menu->wins[index]);
  }

  win_refresh(menu->wins[menu->win_index]);
}

void menu_resize(menu_t* menu, int xmax, int ymax)
{
  switch(menu->type)
  {
    case MENU_DB:
      menu_db_resize((menu_db_t*) menu, xmax, ymax);
      break;

    case MENU_DBS:
      menu_dbs_resize((menu_dbs_t*) menu, xmax, ymax);
      break;

    case MENU_PSW:
      menu_psw_resize((menu_psw_t*) menu, xmax, ymax);
      break;
  }
}

void menu_free(menu_t* menu)
{
  switch(menu->type)
  {
    case MENU_DB:
      menu_db_free((menu_db_t*) menu);
      break;

    case MENU_DBS:
      menu_dbs_free((menu_dbs_t*) menu);
      break;

    case MENU_PSW:
      menu_psw_free((menu_psw_t*) menu);
      break;
  }
}

void menu_head_free(menu_head_t* menu)
{
  if(menu == NULL) return;

  for(int index = 0; index < win->win_count; index++)
  {
    win_free(menu->wins[index]);
  }

  free(menu->wins);

  for(int index = 0; index < win->pop_count; index++)
  {
    win_free(menu->pops[index]);
  }

  free(menu->pops);
}
