#include "../passman.h"

infwin_t* infwin_create(int x, int y, int w, char* title, char* text)
{
  infwin_t* infwin = malloc(sizeof(infwin_t));

  int h = MIN((strlen(text) / (w - 2)) + 3, 0);

  infwin->window = window_create(x, y, w, h);

  infwin->title = title;
  infwin->text  = text;

  return infwin;
}

void infwin_free(infwin_t* infwin)
{
  if(infwin == NULL) return;

  window_free(infwin->window);

  free(infwin);
}

void infwin_refresh(infwin_t* infwin)
{
  window_clean(infwin->window);

  WINDOW* window = infwin->window->window;

  int ymax = infwin->window->ymax;
  int xmax = infwin->window->xmax;

  curs_set(0);

  box(window, 0, 0);

  int shift = (xmax - strlen(infwin->title)) / 2;

  mvwprintw(window, 0, shift, "%s", infwin->title);

  int length = strlen(infwin->text);

  for(int height = 0; height < ymax - 2; height++)
  {
    wmove(window, 1 + height, 1);

    for(int width = 0; width < xmax - 2; width++)
    {
      int index = (height * (xmax - 2) + width);

      if(index >= length) break; 

      waddch(window, infwin->text[index]);
    }
  }

  wrefresh(window);
}
