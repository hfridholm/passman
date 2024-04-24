#include "../passman.h"

/*
 * PARAMS
 * - window_t* window |
 * - int x            | x-value center of window
 * - int y            | y-value center of window
 * - int w            | width of window
 * - int h            | height of window
 *
 * Note: Maybe return window_t* window
 */
void window_resize(window_t* window, int x, int y, int w, int h)
{
  wresize(window->window, h, w);

  mvwin(window->window, y - (h / 2), x - (w / 2));

  window->xmax = w;
  window->ymax = h;
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 * - int h | height of window
 *
 * RETURN (window_t* window)
 */
window_t* window_create(int x, int y, int w, int h)
{
  window_t* window = malloc(sizeof(window_t));

  window->window = newwin(h, w, y - (h / 2), x - (w / 2));
  
  window->xmax = w;
  window->ymax = h;

  keypad(window->window, TRUE);

  window->active = true;

  return window;
}

/*
 * Print a title in the center top of a window
 */
void window_title_center_print(window_t* window, const char* title)
{
  size_t length = strlen(title);

  int x = (window->xmax - length) / 2 - 1;

  mvwprintw(window->window, 0, x, "%s", title);
}

void window_clean(window_t* window)
{
  for(int y = 0; y < window->ymax; y++)
  {
    wmove(window->window, y, 0);

    for(int x = 0; x < window->xmax; x++)
    {
      waddch(window->window, ' ');
    }
  }
}

void window_free(window_t* window)
{
  if(window == NULL) return;

  wclear(window->window);

  wrefresh(window->window);

  delwin(window->window);

  free(window);
}

