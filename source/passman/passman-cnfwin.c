#include "../passman.h"
#include "passman-intern.h"

cnfwin_t* cnfwin_create(int h, int w, int y, int x, char* prompt, char* ytext, char* ntext)
{
  cnfwin_t* cnfwin = malloc(sizeof(cnfwin_t));

  cnfwin->window = newwin(h, w, y, x);

  keypad(cnfwin->window, TRUE);

  cnfwin->ymax = getmaxy(cnfwin->window);
  cnfwin->xmax = getmaxx(cnfwin->window);

  cnfwin->prompt = prompt;
  cnfwin->ytext  = ytext;
  cnfwin->ntext  = ntext;

  return cnfwin;
}

cnfwin_t* cnfwin_center_create(window_t* parent, int w, char* prompt, char* ytext, char* ntext)
{
  int ymax = getmaxy(parent);
  int xmax = getmaxx(parent);

  int length = strlen(prompt);

  int h = (length / (w - 2)) + 5;

  int y = (ymax - h) / 2;
  int x = (xmax - w) / 2;

  return cnfwin_create(h, w, y, x, prompt, ytext, ntext);
}

void cnfwin_free(cnfwin_t* cnfwin)
{
  if(cnfwin == NULL) return;

  window_free(cnfwin->window);
}

void cnfwin_refresh(cnfwin_t* cnfwin)
{
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

void cnfwin_input(cnfwin_t* cnfwin)
{
  cnfwin_refresh(cnfwin);

  int key;
  while((key = wgetch(cnfwin->window)))
  {
    if(key == 10) break;

    switch(key)
    {
      case KEY_RIGHT:
        cnfwin->answer = false;
        break;

      case KEY_LEFT:
        cnfwin->answer = true;
        break;
    }
  
    cnfwin_refresh(cnfwin);
  }
}

void confirm_input(bool* answer, char* prompt, char* ytext, char* ntext)
{
  cnfwin_t* cnfwin = cnfwin_center_create(stdscr, 30, prompt, ytext, ntext);

  refresh();

  cnfwin_input(cnfwin);

  *answer = cnfwin->answer;

  cnfwin_free(cnfwin);
}
