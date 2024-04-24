#include "../passman.h"

char* dbenms[] = {"Secret", "Home", "School", "This", "That", "Sweden", "Hampus", "Nogger"};

lstwin_t* dbases;
inpwin_t* search;

char buffer[64];

void mendbs_refresh(void)
{
  lstwin_refresh(dbases);

  inpwin_refresh(search);
}

void mendbs_resize(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;
  int w = xmax - 12;

  inpwin_resize(search, x, 5, w);

  lstwin_resize(dbases, x, y + 2, w, ymax - 10);
}

void mendbs_init(void)
{
  dbases = lstwin_create(1, 1, 1, 1, dbenms, 8);

  search = inpwin_create(1, 1, 1, buffer, sizeof(buffer), false);
}

void mendbs_free(void)
{
  lstwin_free(dbases);

  inpwin_free(search);
}

void mendbs_search_input(void)
{
  curs_set(1);

  inpwin_input(search, NULL);

  curs_set(0);
}

void mendbs_dbases_key_handler(int key)
{
  switch(key)
  {
    case KEY_ENTR:
      // pswpop_input();

      menpsw_input();

      menu = MENU_DATABASES;
      break;

    case 'd':
      delpop_input();
      break;

    case 'n':
      break;

    case 'r':
      break;

    case KEY_TAB:
      mendbs_search_input();
      break;

    default:
      break;
  }
}

void mendbs_input(void)
{
  menu = MENU_DATABASES;

  lstwin_input(dbases, &mendbs_dbases_key_handler);
}
