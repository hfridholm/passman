#include "../passman.h"
#include "passman-intern.h"

acswin_t* acswin_create(int h, int w, int y, int x)
{
  int ymax = getmaxy(stdscr);

  if((y + h) > ymax) return NULL; 


  acswin_t* acswin = malloc(sizeof(acswin_t));

  acswin->window = newwin(h, w, y, x);


  acswin->ymax = ymax;
  acswin->xmax = getmaxx(acswin->window);

  keypad(acswin->window, TRUE);

  return acswin;
}

void acswin_free(acswin_t* acswin)
{
  if(acswin == NULL) return;

  window_free(acswin->window);

  free(acswin);
}

acswin_t* acswin_center_create(window_t* parent, int h, int y, int x)
{
  int ymax = getmaxy(parent);
  int xmax = getmaxx(parent);

  int w = xmax - x - x;
  if(w <= 0) return NULL;

  if((y + h) > ymax) return NULL;

  return acswin_create(h, w, y, x);
}

void acswin_refresh(acswin_t* acswin)
{
  for(int index = 0; index < database.amount; index++)
  {
    if(index == acswin->index) wattron(acswin->window, A_REVERSE);

    char* account = database.accounts[index].account;

    mvwprintw(acswin->window, index + 1, 1, "%s", account);

    wattroff(acswin->window, A_REVERSE);
  }

  box(acswin->window, 0, 0);

  wrefresh(acswin->window);
}
