#include "../passman.h"

void lstwin_resize(lstwin_t* lstwin, int x, int y, int w, int h)
{
  window_resize(lstwin->window, x, y, w, h);

  lstwin->ymax = h;
  lstwin->xmax = w;
}

lstwin_t* lstwin_create(int x, int y, int w, int h, char** dbases, int amount)
{
  lstwin_t* lstwin = malloc(sizeof(lstwin_t));

  lstwin->window = window_create(x, y, w, h);

  lstwin->ymax = h;
  lstwin->xmax = w;

  keypad(lstwin->window, TRUE);

  lstwin->dbases = dbases;
  lstwin->amount = amount;

  return lstwin;
}

void lstwin_free(lstwin_t* lstwin)
{
  if(lstwin == NULL) return;

  window_free(lstwin->window);

  free(lstwin);
}

void lstwin_refresh(lstwin_t* lstwin)
{
  window_clean(lstwin->window);

  for(int index = 0; index < lstwin->amount; index++)
  {
    if(index == lstwin->index) wattron(lstwin->window, A_REVERSE);

    mvwprintw(lstwin->window, index + 1, 1, "%s", lstwin->dbases[index]);

    wattroff(lstwin->window, A_REVERSE);
  }

  box(lstwin->window, 0, 0);

  wrefresh(lstwin->window);
}

void lstwin_key_handler(lstwin_t* lstwin, int key)
{
  switch(key)
  {
    case 'j':
      lstwin->index = MIN(lstwin->index + 1, lstwin->amount - 1);
      break;

    case 'k':
      lstwin->index = MAX(lstwin->index - 1, 0);
      break;
  }
}
