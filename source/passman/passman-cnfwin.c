#include "../passman.h"
#include "passman-intern.h"

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 */
cnfwin_t* cnfwin_create(int x, int y, int w, char* prompt, char* ytext, char* ntext)
{
  cnfwin_t* cnfwin = malloc(sizeof(cnfwin_t));

  int h = (strlen(prompt) / (w - 2)) + 5;

  cnfwin->window = window_create(x, y, w, h);

  keypad(cnfwin->window, TRUE);

  cnfwin->ymax = h;
  cnfwin->xmax = w;

  cnfwin->prompt = prompt;
  cnfwin->ytext  = ytext;
  cnfwin->ntext  = ntext;

  return cnfwin;
}

void cnfwin_free(cnfwin_t* cnfwin)
{
  if(cnfwin == NULL) return;

  window_free(cnfwin->window);

  free(cnfwin);
}

void cnfwin_refresh(cnfwin_t* cnfwin)
{
  window_clean(cnfwin->window);

  curs_set(0);

  box(cnfwin->window, 0, 0);

  int length = strlen(cnfwin->prompt);
  int index = 0;

  for(int height = 0; height < cnfwin->ymax - 4; height++)
  {
    if(height >= cnfwin->ymax - 5)
    {
      int rlength = strlen(cnfwin->prompt + index);
      int cshift = (cnfwin->xmax - rlength) / 2;

      wmove(cnfwin->window, 1 + height, cshift);
    }
    else wmove(cnfwin->window, 1 + height, 1);

    for(int width = 0; width < cnfwin->xmax - 2; width++)
    {
      index = (height * (cnfwin->xmax - 2) + width);

      if(index >= length) break; 

      waddch(cnfwin->window, cnfwin->prompt[index]);
    }
  }

  int ylength = strlen(cnfwin->ytext);
  int nlength = strlen(cnfwin->ntext);

  int alength = ylength + 1 + nlength;

  int ashift = (cnfwin->xmax - alength) / 2;

  if(cnfwin->answer == true) wattron(cnfwin->window, A_REVERSE);

  wmove(cnfwin->window, cnfwin->ymax - 2, ashift);

  wprintw(cnfwin->window, "%s", cnfwin->ytext);
  
  wattroff(cnfwin->window, A_REVERSE);

  waddch(cnfwin->window, ' ');

  if(cnfwin->answer == false) wattron(cnfwin->window, A_REVERSE);

  wprintw(cnfwin->window, "%s", cnfwin->ntext);

  wattroff(cnfwin->window, A_REVERSE);

  wrefresh(cnfwin->window);
}

void cnfwin_key_handler(cnfwin_t* cnfwin, int key)
{
  switch(key)
  {
    case KEY_RIGHT:
      cnfwin->answer = false;
      break;

    case KEY_LEFT:
      cnfwin->answer = true;
      break;
  }
}
