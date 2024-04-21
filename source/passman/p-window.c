#include "../passman.h"

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 * - int h | height of window
 */
void window_resize(window_t* window, int x, int y, int w, int h)
{
  wresize(window, h, w);

  mvwin(window, y - (h / 2), x - (w / 2));
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 * - int h | height of window
 */
window_t* window_create(int x, int y, int w, int h)
{
  return newwin(h, w, y - (h / 2), x - (w / 2));
}

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

void window_clean(window_t* window)
{
  int ymax = getmaxy(window);
  int xmax = getmaxx(window);

  for(int y = 0; y < ymax; y++)
  {
    wmove(window, y, 0);

    for(int x = 0; x < xmax; x++)
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

