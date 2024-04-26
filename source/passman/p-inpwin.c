#include "../passman.h"

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 */
void inpwin_resize(inpwin_t* inpwin, int x, int y, int w)
{
  window_resize(inpwin->window, x, y, w, 3);

  // 1. Clamp the cursor to the end
  if((inpwin->cursor - inpwin->scroll) >= (w - 2))
  {
    inpwin->scroll = (inpwin->cursor - (w - 2) + 1);
  }

  // 2. Clamp the marked item to the ceiling
  if(inpwin->cursor < inpwin->scroll)
  {
    inpwin->scroll = inpwin->cursor;
  }

  // 3. Prevent empty space when text can occupy the space
  if(inpwin->scroll + (w - 2) > inpwin->length)
  {
    if(inpwin->cursor == inpwin->length)
    {
      inpwin->scroll = MAX(0, inpwin->length - (w - 2) + 1);
    }
    else inpwin->scroll = MAX(0, inpwin->length - (w - 2));
  }
}

/*
 * Sync the input window with a new buffer and update cursor
 */
void inpwin_buffer_set(inpwin_t* inpwin, char* buffer, size_t size)
{
  inpwin->buffer = buffer;

  if(!buffer) return;

  inpwin->msize = size - 1;

  inpwin->length = strlen(buffer);
  inpwin->cursor = inpwin->length;

  int xmax = inpwin->window->xmax;

  inpwin->scroll = MAX(0, inpwin->length - (xmax - 2) + 1);
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 *
 * RETURN (inpwin_t* inpwin)
 */
inpwin_t* inpwin_create(int x, int y, int w, char* buffer, size_t size, char* title, bool secret, bool active)
{
  inpwin_t* inpwin = malloc(sizeof(inpwin_t));

  inpwin->window = window_create(x, y, w, 3, active);

  inpwin_buffer_set(inpwin, buffer, size);

  inpwin->secret = secret;
  inpwin->hidden = secret;

  inpwin->title  = title;
  inpwin->ttllen = (title ? strlen(title) : 0);

  return inpwin;
}

/*
 *
 */
void inpwin_free(inpwin_t* inpwin)
{
  if(inpwin == NULL) return;

  window_free(inpwin->window);

  free(inpwin);
}

/*
 *
 */
static void inpwin_buffer_print(inpwin_t* inpwin)
{
  WINDOW* window = inpwin->window->window;

  int xmax = inpwin->window->xmax;

  wmove(window, 1, 1);

  // The amount of characters to print
  int amount = MIN(inpwin->length, xmax - 2);

  for(int index = 0; index < amount; index++)
  {
    int pindex = inpwin->scroll + index;

    bool hidden = (inpwin->hidden && inpwin->secret);

    char symbol = (hidden ? '*' : inpwin->buffer[pindex]);

    if(symbol == '\0') symbol = ' ';

    waddch(window, symbol);
  }
}

/*
 *
 */
static void inpwin_length_print(inpwin_t* inpwin)
{
  WINDOW* window = inpwin->window->window;

  int xmax = inpwin->window->xmax;

  mvwprintw(window, 2, xmax - 8, "%03d/%03d", inpwin->length, inpwin->msize);
}

/*
 * Print the title of the text window
 */
static void inpwin_title_print(inpwin_t* inpwin)
{
  if(!inpwin->title || !inpwin->ttllen) return;

  WINDOW* window = inpwin->window->window;

  int xmax = inpwin->window->xmax;

  int length = MIN(xmax - 2, inpwin->ttllen);

  wmove(window, 0, (xmax - length) / 2);

  for(int index = 0; index < length; index++)
  {
    waddch(window, inpwin->title[index]);
  }
}

/*
 * Refresh the content of the buffer being shown
 */
void inpwin_refresh(inpwin_t* inpwin)
{
  if(!inpwin->window->active) return;

  window_clean(inpwin->window);

  if(inpwin->buffer)
  {
    inpwin_buffer_print(inpwin);
  }

  WINDOW* window = inpwin->window->window;

  box(window, 0, 0);

  if(inpwin->title)
  {
    inpwin_title_print(inpwin);
  }

  inpwin_length_print(inpwin);

  wmove(window, 1, 1 + (inpwin->cursor - inpwin->scroll));

  wrefresh(window);
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Symbol not supported
 * - 2 | Buffer is full
 */
static int inpwin_symbol_add(inpwin_t* inpwin, char symbol)
{
  if(symbol < 32 || symbol > 126) return 1;

  if(inpwin->length >= inpwin->msize) return 2;

  int xmax = inpwin->window->xmax;

  // Shift characters forward to make room for new character
  for(int index = inpwin->length + 1; index-- > inpwin->cursor;)
  {
    inpwin->buffer[index] = inpwin->buffer[index - 1];
  }

  inpwin->buffer[inpwin->cursor] = symbol;

  inpwin->length++;

  inpwin->cursor = MIN(inpwin->cursor + 1, inpwin->length);

  // The cursor is at the end of the input window
  if((inpwin->cursor - inpwin->scroll) >= (xmax - 2))
  {
    inpwin->scroll++; // Scroll one more character
  }

  return 0; // Success!
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | No symbol to delete
 */
static int inpwin_symbol_del(inpwin_t* inpwin)
{
  if(inpwin->cursor <= 0) return 1;

  // Fill in the room left by the deleted character
  for(int index = inpwin->cursor - 1; index < inpwin->length; index++)
  {
    inpwin->buffer[index] = inpwin->buffer[index + 1];
  }

  inpwin->length = MAX(0, inpwin->length - 1);

  inpwin->buffer[inpwin->length] = '\0';

  inpwin->cursor = MIN(inpwin->cursor - 1, inpwin->length);

  return 0; // Success!
}

/*
 *
 */
static void inpwin_scroll_right(inpwin_t* inpwin)
{
  // The cursor can not be further than the text itself
  inpwin->cursor = MIN(inpwin->length, inpwin->cursor + 1);

  int xmax = inpwin->window->xmax;

  // The cursor is at the end of the input window
  if((inpwin->cursor - inpwin->scroll) >= (xmax - 2))
  {
    inpwin->scroll++; // Scroll one more character
  }
}

/*
 *
 */
static void inpwin_scroll_left(inpwin_t* inpwin)
{
  inpwin->cursor = MAX(0, inpwin->cursor - 1);

  // If the cursor is to the left of the window,
  // scroll to the start of the cursor
  if(inpwin->cursor < inpwin->scroll)
  {
    inpwin->scroll = inpwin->cursor;
  }
}

/*
 * Note: If input window has no buffer, nothing should be done
 */
static void inpwin_key_handler(inpwin_t* inpwin, int key)
{
  if(!inpwin->buffer) return;

  switch(key)
  {
    case KEY_CTRLH:
      if(inpwin->secret) inpwin->hidden = !inpwin->hidden;
      break;

    case KEY_RIGHT:
      inpwin_scroll_right(inpwin);
      break;

    case KEY_LEFT:
      inpwin_scroll_left(inpwin);
      break;

    case KEY_BACKSPACE:
      inpwin_symbol_del(inpwin);
      break;
    
    default:
      inpwin_symbol_add(inpwin, key);
      break;
  }
}

/*
 * PARAMS
 * - inpwin_t* inpwin         | The input window to input to
 * - void (*key_handler)(int) | A possible custom key handler
 */
void inpwin_input(inpwin_t* inpwin, void (*key_handler)(int))
{
  if(!inpwin->window->active) return;

  curs_set(1);

  screen_refresh();

  WINDOW* window = inpwin->window->window;

  int key;
  while(running && (key = wgetch(window)))
  {
    curs_set(0);

    screen_key_handler(key);

    inpwin_key_handler(inpwin, key);

    if(key_handler) key_handler(key);
    
    else if(key == KEY_ENTR) break;

    curs_set(1);

    screen_refresh();
  }

  curs_set(0);
}

/*
 * Open a popup, input and then close the popup again
 *
 * PARAMS
 * - inpwin_t* inppop         | The input popup to input to
 * - void (*key_handler)(int) | A possible custom key handler
 */
void inppop_input(inpwin_t* inppop, void (*key_handler)(int))
{
  inppop->window->active = true;

  inpwin_input(inppop, key_handler);

  inppop->window->active = false;
}
