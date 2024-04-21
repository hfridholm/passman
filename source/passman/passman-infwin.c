#include "../passman.h"
#include "passman-intern.h"

infwin_t* infwin_create(int h, int w, int y, int x, char* title, char* text)
{
  infwin_t* infwin = malloc(sizeof(infwin_t));

  infwin->window = newwin(h, w, y, x);

  infwin->ymax = getmaxy(infwin->window);
  infwin->xmax = getmaxx(infwin->window);

  infwin->title = title;
  infwin->text  = text;

  return infwin;
}

void infwin_free(infwin_t* infwin)
{
  if(infwin == NULL) return;

  window_free(infwin->window);
}

infwin_t* infwin_center_create(window_t* parent, int w, char* title, char* text)
{
  int ymax = getmaxy(parent);
  int xmax = getmaxx(parent);

  int length = strlen(text);

  int h = (length / (w - 2)) + 3;

  int y = (ymax - h) / 2;
  int x = (xmax - w) / 2;

  return infwin_create(h, w, y, x, title, text);
}

void infwin_refresh(infwin_t* infwin)
{
  curs_set(0);

  box(infwin->window, 0, 0);

  int shift = (infwin->xmax - strlen(infwin->title)) / 2;

  mvwprintw(infwin->window, 0, shift, "%s", infwin->title);

  int length = strlen(infwin->text);

  for(int height = 0; height < infwin->ymax - 2; height++)
  {
    wmove(infwin->window, 1 + height, 1);

    for(int width = 0; width < infwin->xmax - 2; width++)
    {
      int index = (height * (infwin->xmax - 2) + width);

      if(index >= length) break; 

      waddch(infwin->window, infwin->text[index]);
    }
  }

  wrefresh(infwin->window);
}

void info_print(char* title, char* text)
{
  int xmax = getmaxx(stdscr);
  int w = MIN(xmax - 8, 20);

  infwin_t* infwin = infwin_center_create(stdscr, w, title, text);

  refresh();
  infwin_refresh(infwin);

  getch();

  infwin_free(infwin);
}
