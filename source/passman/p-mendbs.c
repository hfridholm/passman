#include "../passman.h"

void mendbs_resize(void)
{
  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  int x = xmax / 2;
  int w = xmax - 12;

  inpwin_resize(mendbs.search, x, 5, w);

  dbswin_resize(mendbs.dbases, x, (ymax / 2) + 2, w, ymax - 10);

  cnfwin_resize(mendbs.delete, x, (ymax / 2), 20);
}

void mendbs_refresh(void)
{
  refresh();

  dbswin_refresh(mendbs.dbases);

  inpwin_refresh(mendbs.search, false);

  if(mendbs.popup == MENDBS_POPUP_DELETE)
  {
    cnfwin_refresh(mendbs.delete);
  }
}

char* dbases[] = {"Secret", "Home", "School"};

void mendbs_init(void)
{
  mendbs.dbases = dbswin_create(1, 1, 1, 1,
    dbases, 3);

  mendbs.search = inpwin_create(1, 1, 1,
    mendbs.buffer, sizeof(mendbs.buffer));

  mendbs.delete = cnfwin_create(1, 1, 1,
    "Delete Database?", "Yes", "No");

  mendbs_resize();
}

void mendbs_free(void)
{
  dbswin_free(mendbs.dbases);

  inpwin_free(mendbs.search);

  cnfwin_free(mendbs.delete);
}

void mendbs_search_input(void)
{
  curs_set(1);

  mendbs_refresh();

  int key;
  while((key = wgetch(mendbs.search->window)))
  {
    if(key == 10) break;

    inpwin_key_handler(mendbs.search, key);

    if(key == KEY_RESIZE) mendbs_resize();
  
    mendbs_refresh();
  }
  curs_set(0);
}

void mendbs_delete_input(void)
{
  mendbs_refresh();
  cnfwin_refresh(mendbs.delete);

  int key;
  while((key = wgetch(mendbs.delete->window)))
  {
    move(1, 0);
    printw("%03d", key);
    refresh();

    if(key == 10) break;

    cnfwin_key_handler(mendbs.delete, key);

    if(key == KEY_RESIZE) mendbs_resize();
  
    mendbs_refresh();
    cnfwin_refresh(mendbs.delete);
  }
}

void mendbs_dbases_key_handler(int key)
{
  switch(key)
  {
    case 'd':
      move(0, 0);
      printw("delete");
      refresh();

      mendbs_delete_input();

      break;

    case 'n':
      move(0, 0);
      printw("new");
      refresh();
      break;

    case 'o':
      move(0, 0);
      printw("open");
      refresh();
      break;

    case 'r':
      move(0, 0);
      printw("rename");
      refresh();
      break;

    case '/':
      move(0, 0);
      printw("search");
      refresh();

      mendbs_search_input();

      break;

    case KEY_RESIZE:
      mendbs_resize();
      break;
  }
}

void mendbs_input(void)
{
  mendbs_refresh();

  int key;
  while((key = wgetch(mendbs.dbases->window)))
  {
    if(key == 10) break;

    dbswin_key_handler(mendbs.dbases, key);

    mendbs_dbases_key_handler(key);

    move(1, 0);
    printw("%03d", key);
    refresh();

    mendbs_refresh();
  }
}
