#include "../passman.h"

char* dbases[] = {"Secret", "Home", "School", "This", "That", "Sweden", "Hampus", "Nogger"};

void mendbs_refresh(void)
{
  lstwin_refresh(mendbs.dbases);

  inpwin_refresh(mendbs.search, false);
}

void mendbs_resize(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;
  int w = xmax - 12;

  inpwin_resize(mendbs.search, x, 5, w);

  lstwin_resize(mendbs.dbases, x, y + 2, w, ymax - 10);
}

void mendbs_init(void)
{
  mendbs.dbases = lstwin_create(1, 1, 1, 1,
    dbases, 8);

  mendbs.search = inpwin_create(1, 1, 1,
    mendbs.buffer, sizeof(mendbs.buffer));
}

void mendbs_free(void)
{
  lstwin_free(mendbs.dbases);

  inpwin_free(mendbs.search);
}

void mendbs_search_input(void)
{
  curs_set(1);

  screen_refresh();

  int key;
  while(running && (key = wgetch(mendbs.search->window)))
  {
    if(key == KEY_ENTR) break;

    screen_key_handler(key);

    inpwin_key_handler(mendbs.search, key);

    screen_refresh();
  }

  curs_set(0);
}

void mendbs_dbases_key_handler(int key)
{
  switch(key)
  {
    case 'd':
      delpop_input();
      break;

    case 'n':
      move(0, 0);
      printw("new");
      break;

    case 'r':
      move(0, 0);
      printw("rename");
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

  screen_refresh();

  int key;
  while(running && (key = wgetch(mendbs.dbases->window)))
  {
    if(key == KEY_ENTR)
    {
      // pswpop_input();

      move(1, 0);
      printw("Opening database: %d", mendbs.dbases->index);

      move(2, 0);
      printw("Filepath: %s", mendbs.dbases->items[mendbs.dbases->index]);

      menpsw_input();

      menu = MENU_DATABASES;
    }

    screen_key_handler(key);

    lstwin_key_handler(mendbs.dbases, key);

    mendbs_dbases_key_handler(key);

    screen_refresh();

    move(0, 0);
    printw("%03d", key);
    refresh();
  }
}
