#include "../passman.h"

/*
 * Calculate the height of the window depending on text and width
 *
 * PARAMS
 * - int txtlen | Length of text
 * - int w      | The width of the window
 *
 * RETURN (int h)
 */
static int infwin_height(int txtlen, int w)
{
  return MAX((txtlen / (w - 2)) + 3, 0);
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 * - int h | height of window
 *
 * Note: If the height is invalid, a new height will be calculated
 */
void infwin_resize(infwin_t* infwin, int x, int y, int w, int h)
{
  if(h <= 0) h = infwin_height(infwin->txtlen, w);

  window_resize(infwin->window, x, y, w, h);
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 * - int h | height of window
 *
 * Note: If the height is invalid, a new height will be calculated
 *
 * RETURN (infwin_t* infwin)
 */
infwin_t* infwin_create(int x, int y, int w, int h, char* title, char* text, bool active)
{
  infwin_t* infwin = malloc(sizeof(infwin_t));

  infwin->title  = title;
  infwin->ttllen = (title ? strlen(title) : 0);

  infwin->text   = text;
  infwin->txtlen = (text ? strlen(text) : 0);

  if(h <= 0) h = infwin_height(infwin->txtlen, w);

  infwin->window = window_create(x, y, w, h, active);

  return infwin;
}

/*
 *
 */
void infwin_free(infwin_t* infwin)
{
  if(infwin == NULL) return;

  window_free(infwin->window);

  free(infwin);
}

/*
 *
 */
static void infwin_text_print(infwin_t* infwin)
{
  if(!infwin->text || !infwin->txtlen) return;

  WINDOW* window = infwin->window->window;

  int ymax = infwin->window->ymax;
  int xmax = infwin->window->xmax;

  int lines = MIN(ymax - 2, infwin->txtlen / (xmax - 2) + 1);

  int yshift = MAX(0, ((ymax - 2) - lines) / 2);

  int index = 0;
  for(int height = 0; height < lines; height++)
  {
    // 1. Shift text to center of window
    if(height >= (lines - 1))
    {
      int xshift = (xmax - infwin->txtlen + index) / 2;

      wmove(window, yshift + 1 + height, xshift);
    }
    else wmove(window, yshift + 1 + height, 1);
    
    // 2. Print current line of text
    for(int width = 0; width < xmax - 2; width++)
    {
      index = (height * (xmax - 2) + width);

      if(index >= infwin->txtlen) break; 

      waddch(window, infwin->text[index]);
    }
  }
}

/*
 * Print the title of the info window
 */
static void infwin_title_print(infwin_t* infwin)
{
  if(!infwin->title || !infwin->ttllen) return;

  WINDOW* window = infwin->window->window;

  int xmax = infwin->window->xmax;

  int shift = (xmax - infwin->ttllen) / 2;

  mvwprintw(window, 0, shift, "%s", infwin->title);
}

/*
 *
 */
void infwin_refresh(infwin_t* infwin)
{
  if(!infwin->window->active) return;

  window_clean(infwin->window);

  WINDOW* window = infwin->window->window;

  box(window, 0, 0);

  infwin_title_print(infwin);

  infwin_text_print(infwin);

  wrefresh(window);
}

/*
 * PARAMS
 * - infwin_t* infwin         | The info window to input to
 * - void (*key_handler)(int) | A possible custom key handler
 */
void infwin_input(infwin_t* infwin, void (*key_handler)(int))
{
  if(!infwin->window->active) return;

  screen_refresh();

  int key;
  while(running && (key = wgetch(infwin->window->window)))
  {
    screen_key_handler(key);

    if(key_handler) key_handler(key);
    
    else if(key == KEY_ENTR) break;

    screen_refresh();
  }
}

/*
 * Open a popup, input and then close the popup again
 *
 * PARAMS
 * - infwin_t* infpop         | The info popup to input to
 * - void (*key_handler)(int) | A possible custom key handler
 */
void infpop_input(infwin_t* infpop, void (*key_handler)(int))
{
  infpop->window->active = true;

  infwin_input(infpop, key_handler);

  infpop->window->active = false;
}
