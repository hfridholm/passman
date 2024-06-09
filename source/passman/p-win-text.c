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
static int win_text_height(int txtlen, int w)
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
void win_text_resize(win_text_t* win, int x, int y, int w, int h)
{
  if(h <= 0) h = win_text_height(win->text_len, w);

  win_head_resize((win_head_t*) win, x, y, w, h);
}

/*
 * Note: After you change the title, you have to resize the window
 */
void win_text_title_set(win_text_t* win, char* title)
{
  win->title = title;

  win->title_len = (title ? strlen(title) : 0);
}

/*
 * Note: After you change the text, you have to resize the window
 */
void win_text_text_set(win_text_t* win, char* text)
{
  win->text = text;

  win->text_len = (text ? strlen(text) : 0);
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
 * RETURN (win_text_t* win)
 */
win_text_t* win_text_create(char* name, int x, int y, int w, int h, char* title, char* text, bool active, key_handler_t* key_handler)
{
  win_text_t* win = malloc(sizeof(win_text_t));

  win_text_title_set(win, title);

  win_text_text_set(win, text);

  if(h <= 0) h = win_text_height(win->text_len, w);

  win->head = win_head_create(WIN_TEXT, name, x, y, w, h, active, key_handler);

  return win;
}

/*
 *
 */
void win_text_free(win_text_t* win)
{
  if(win == NULL) return;

  win_head_free(win->head);

  free(win);
}

/*
 *
 */
static void win_text_text_print(win_text_t* win)
{
  if(!win->text || !win->text_len) return;

  WINDOW* window = win->head.window;

  int ymax = win->head.ymax;
  int xmax = win->head.xmax;

  int lines = MIN(ymax - 2, (win->text_len / (xmax - 2)) + 1);

  int yshift = MAX(0, ((ymax - 2) - lines) / 2);

  int index = 0;
  for(int height = 0; height < lines; height++)
  {
    // 1. Shift text to center of window
    if(height == (lines - 1))
    {
      int line_len = MIN(xmax - 2, win->text_len - index);

      int xshift = (xmax - line_len) / 2;
      
      wmove(window, yshift + 1 + height, xshift);
    }
    else wmove(window, yshift + 1 + height, 1);
    
    // 2. Print current line of text
    for(int width = 0; width < xmax - 2; width++)
    {
      index = (height * (xmax - 2) + width);

      if(index >= win->text_len) break; 

      waddch(window, win->text[index]);
    }
  }
}

/*
 * Print the title of the text window
 */
static void win_text_title_print(win_text_t* win)
{
  if(!win->title || !win->title_len) return;

  WINDOW* window = win->head.window;

  int xmax = win->head.xmax;

  int length = MIN(xmax - 2, win->title_len);

  wmove(window, 0, (xmax - length) / 2);

  for(int index = 0; index < length; index++)
  {
    waddch(window, win->title[index]);
  }
}

/*
 *
 */
void win_text_refresh(win_text_t* win)
{
  if(!win->head.active) return;

  win_head_clean((win_head_t*) win);

  WINDOW* window = win->head.window;

  box(window, 0, 0);

  win_text_title_print(win);

  win_text_text_print(win);

  wrefresh(window);
}

void pop_text_key_handler(win_head_t* win_head, int key)
{
  if(win_head == NULL || win_head->type != WIN_TEXT) return;

  switch(key)
  {
    case KEY_ENTER:
      win_head->active = false;
      break;
  }
}
