#include "../passman.h"
#include "passman-intern.h"

void dbswin_resize(dbswin_t* dbswin, int x, int y, int w, int h)
{
  window_resize(dbswin->window, x, y, w, h);

  dbswin->ymax = h;
  dbswin->xmax = w;
}

dbswin_t* dbswin_create(int x, int y, int w, int h, char** dbases, int amount)
{
  dbswin_t* dbswin = malloc(sizeof(dbswin_t));

  dbswin->window = window_create(x, y, w, h);

  dbswin->ymax = h;
  dbswin->xmax = w;

  keypad(dbswin->window, TRUE);

  dbswin->dbases = dbases;
  dbswin->amount = amount;

  return dbswin;
}

void dbswin_free(dbswin_t* dbswin)
{
  if(dbswin == NULL) return;

  window_free(dbswin->window);

  free(dbswin);
}

void dbswin_refresh(dbswin_t* dbswin)
{
  window_clean(dbswin->window);

  for(int index = 0; index < dbswin->amount; index++)
  {
    if(index == dbswin->index) wattron(dbswin->window, A_REVERSE);

    mvwprintw(dbswin->window, index + 1, 1, "%s", dbswin->dbases[index]);

    wattroff(dbswin->window, A_REVERSE);
  }

  box(dbswin->window, 0, 0);

  wrefresh(dbswin->window);
}

void dbswin_key_handler(dbswin_t* dbswin, int key)
{
  switch(key)
  {
    case 'j':
      dbswin->index = MIN(dbswin->index + 1, dbswin->amount - 1);
      break;

    case 'k':
      dbswin->index = MAX(dbswin->index - 1, 0);
      break;
  }
}
