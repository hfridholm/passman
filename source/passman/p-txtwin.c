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
static int txtwin_height(int txtlen, int w)
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
void txtwin_resize(txtwin_t* txtwin, int x, int y, int w, int h)
{
  if(h <= 0) h = txtwin_height(txtwin->txtlen, w);

  window_resize(txtwin->window, x, y, w, h);
}

/*
 *
 */
void txtwin_title_set(txtwin_t* txtwin, char* title)
{
  txtwin->title = title;

  txtwin->ttllen = (title ? strlen(title) : 0);
}

/*
 *
 */
void txtwin_text_set(txtwin_t* txtwin, char* text)
{
  txtwin->text = text;

  txtwin->txtlen = (text ? strlen(text) : 0);
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
 * RETURN (txtwin_t* txtwin)
 */
txtwin_t* txtwin_create(int x, int y, int w, int h, char* title, char* text, bool active)
{
  txtwin_t* txtwin = malloc(sizeof(txtwin_t));

  txtwin_title_set(txtwin, title);

  txtwin_text_set(txtwin, text);

  if(h <= 0) h = txtwin_height(txtwin->txtlen, w);

  txtwin->window = window_create(x, y, w, h, active);

  return txtwin;
}

/*
 *
 */
void txtwin_free(txtwin_t* txtwin)
{
  if(txtwin == NULL) return;

  window_free(txtwin->window);

  free(txtwin);
}

/*
 *
 */
static void txtwin_text_print(txtwin_t* txtwin)
{
  if(!txtwin->text || !txtwin->txtlen) return;

  WINDOW* window = txtwin->window->window;

  int ymax = txtwin->window->ymax;
  int xmax = txtwin->window->xmax;

  int lines = MIN(ymax - 2, (txtwin->txtlen / (xmax - 2)) + 1);

  int yshift = MAX(0, ((ymax - 2) - lines) / 2);

  int index = 0;
  for(int height = 0; height < lines; height++)
  {
    // 1. Shift text to center of window
    if(height == (lines - 1))
    {
      int xshift = (xmax - txtwin->txtlen + index) / 2;
      
      wmove(window, yshift + 1 + height, xshift);
    }
    else wmove(window, yshift + 1 + height, 1);
    
    // 2. Print current line of text
    for(int width = 0; width < xmax - 2; width++)
    {
      index = (height * (xmax - 2) + width);

      if(index >= txtwin->txtlen) break; 

      waddch(window, txtwin->text[index]);
    }
  }
}

/*
 * Print the title of the text window
 */
static void txtwin_title_print(txtwin_t* txtwin)
{
  if(!txtwin->title || !txtwin->ttllen) return;

  WINDOW* window = txtwin->window->window;

  int xmax = txtwin->window->xmax;

  int shift = (xmax - txtwin->ttllen) / 2;

  mvwprintw(window, 0, shift, "%s", txtwin->title);
}

/*
 *
 */
void txtwin_refresh(txtwin_t* txtwin)
{
  if(!txtwin->window->active) return;

  window_clean(txtwin->window);

  WINDOW* window = txtwin->window->window;

  box(window, 0, 0);

  txtwin_title_print(txtwin);

  txtwin_text_print(txtwin);

  wrefresh(window);
}

/*
 * PARAMS
 * - txtwin_t* txtwin         | The text window to input to
 * - void (*key_handler)(int) | A possible custom key handler
 */
void txtwin_input(txtwin_t* txtwin, void (*key_handler)(int))
{
  if(!txtwin->window->active) return;

  screen_refresh();

  int key;
  while(running && (key = wgetch(txtwin->window->window)))
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
 * - txtwin_t* txtpop         | The text popup to input to
 * - void (*key_handler)(int) | A possible custom key handler
 */
void txtpop_input(txtwin_t* txtpop, void (*key_handler)(int))
{
  txtpop->window->active = true;

  txtwin_input(txtpop, key_handler);

  txtpop->window->active = false;
}
