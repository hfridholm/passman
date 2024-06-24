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
int win_text_title_set(win_text_t* win, char* title)
{
  if(!win) return 1;

  win->title     = title;
  win->title_len = (title ? strlen(title) : 0);

  return 0;
}

/*
 * Note: After you change the text, you have to resize the window
 */
int win_text_text_set(win_text_t* win, char* text)
{
  if(!win) return 1;

  win->text     = text;
  win->text_len = (text ? strlen(text) : 0);

  return 0;
}

/*
 * Note: After you change the title and text, you have to resize the window
 */
int win_text_title_text_set(win_text_t* win, char* title, char* text)
{
  if(!win) return 1;

  win->text     = text;
  win->text_len = (text ? strlen(text) : 0);

  win->title     = title;
  win->title_len = (title ? strlen(title) : 0);

  return 0;
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
win_text_t* win_text_create(char* name, bool active, bool tab_ability, int x, int y, int w, int h, char* title, char* text, win_event_t* event)
{
  win_text_t* win = malloc(sizeof(win_text_t));

  win_text_title_set(win, title);

  win_text_text_set(win, text);

  if(h <= 0) h = win_text_height(win->text_len, w);

  win->head = win_head_create(WIN_TEXT, name, active, tab_ability, x, y, w, h, event);

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

  int lines = MIN(win->head.h - 2, (win->text_len / (win->head.w - 2)) + 1);

  int yshift = MAX(0, ((win->head.h - 2) - lines) / 2);

  int index = 0;
  for(int height = 0; height < lines; height++)
  {
    // 1. Shift text to center of window
    if(height == (lines - 1))
    {
      int line_len = MIN(win->head.w - 2, win->text_len - index);

      int xshift = (win->head.w - line_len) / 2;
      
      wmove(window, yshift + 1 + height, xshift);
    }
    else wmove(window, yshift + 1 + height, 1);
    
    // 2. Print current line of text
    for(int width = 0; width < win->head.w - 2; width++)
    {
      index = (height * (win->head.w - 2) + width);

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

  if(window == NULL) return;

  int length = MIN(win->head.w - 2, win->title_len);

  wmove(window, 0, (win->head.w - length) / 2);

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

  WINDOW* window = win->head.window;

  if(!window) return;

  win_border_print((win_t*) win);

  win_text_title_print(win);

  win_text_text_print(win);

  wrefresh(window);
}

int pop_text_event(win_head_t* win_head, int key)
{
  if(win_head == NULL || win_head->type != WIN_TEXT) return 0;

  switch(key)
  {
    case KEY_ENTR:
      win_head->active = false;
      return 1;

    default:
      return 0;
  }
}

win_text_t* wins_name_win_text_get(win_t** wins, int count, char* name)
{
  win_t* win = wins_name_win_get(wins, count, name);

  if(!win || win->type != WIN_TEXT) return NULL;

  return (win_text_t*) win;
}
