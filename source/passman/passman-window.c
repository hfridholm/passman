#include "../passman.h"

/*
 * Print a title in the center top of a window
 */
void window_title_center_print(window_t* window, const char* title)
{
  int xmax = getmaxx(window);

  size_t length = strlen(title);

  int x = (xmax - length) / 2 - 1;

  mvwprintw(window, 0, x, "%s", title);
}

void window_inside_clear(window_t* window)
{
  int ymax = getmaxy(window);
  int xmax = getmaxx(window);

  for(int y = 1; y < ymax - 1; y++)
  {
    wmove(window, y, 1);

    for(int x = 1; x < xmax - 1; x++)
    {
      waddch(window, ' ');
    }
  }
}

void window_free(window_t* window)
{
  if(window == NULL) return;

  wclear(window);

  wrefresh(window);

  delwin(window);
}

