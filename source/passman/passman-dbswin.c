#include "../passman.h"
#include "passman-intern.h"

dbswin_t* dbswin_create(int h, int w, int y, int x, char** dbases, int amount)
{
  int ymax = getmaxy(stdscr);

  if((y + h) > ymax) return NULL; 


  dbswin_t* dbswin = malloc(sizeof(dbswin_t));

  dbswin->window = newwin(h, w, y, x);


  dbswin->ymax = ymax;
  dbswin->xmax = getmaxx(dbswin->window);

  keypad(dbswin->window, TRUE);


  dbswin->dbases = dbases; // Copy the memory instead
  dbswin->amount = amount;


  return dbswin;
}

void dbswin_free(dbswin_t* dbswin)
{
  if(dbswin == NULL) return;

  window_free(dbswin->window);

  free(dbswin);
}

dbswin_t* dbswin_center_create(window_t* parent, int h, int y, int x, char** dbases, int amount)
{
  int ymax = getmaxy(parent);
  int xmax = getmaxx(parent);

  int w = xmax - x - x;
  if(w <= 0) return NULL;

  if((y + h) > ymax) return NULL;

  return dbswin_create(h, w, y, x, dbases, amount);
}

void dbswin_refresh(dbswin_t* dbswin)
{
  for(int index = 0; index < dbswin->amount; index++)
  {
    if(index == dbswin->index) wattron(dbswin->window, A_REVERSE);

    mvwprintw(dbswin->window, index + 1, 1, "%s", dbswin->dbases[index]);

    wattroff(dbswin->window, A_REVERSE);
  }

  box(dbswin->window, 0, 0);

  wrefresh(dbswin->window);
}
