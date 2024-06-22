#include "../passman.h"

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 */
void win_input_resize(win_input_t* win, int x, int y, int w)
{
  win_head_resize((win_head_t*) win, x, y, w, 3);

  // 1. Clamp the cursor to the end
  if((win->cursor - win->scroll) >= (w - 2))
  {
    win->scroll = (win->cursor - (w - 2) + 1);
  }

  // 2. Clamp the marked item to the ceiling
  if(win->cursor < win->scroll)
  {
    win->scroll = win->cursor;
  }

  // 3. Prevent empty space when text can occupy the space
  if(win->scroll + (w - 2) > win->buffer_len)
  {
    if(win->cursor == win->buffer_len)
    {
      win->scroll = MAX(0, win->buffer_len - (w - 2) + 1);
    }
    else win->scroll = MAX(0, win->buffer_len - (w - 2));
  }
}

void win_input_buffer_update(win_input_t* win)
{
  win->buffer_len = win->buffer ? strlen(win->buffer) : 0;

  win->cursor = win->buffer_len;

  int xmax = win->head.xmax;

  win->scroll = MAX(0, win->buffer_len - (xmax - 2) + 1);
}

void win_input_buffer_paste(win_input_t* win, const char* string)
{
  if(!win || !string || !win->buffer) return;

  strncpy(win->buffer, string, win->buffer_size);

  win_input_buffer_update(win);
}

/*
 * Sync the input window with a new buffer and update cursor
 */
void win_input_buffer_set(win_input_t* win, char* buffer, size_t size)
{
  if(!win || !buffer) return;

  win->buffer = buffer;

  win->buffer_size = size - 1;

  win_input_buffer_update(win);
}

void win_input_buffer_clear(win_input_t* win)
{
  if(!win) return;

  memset(win->buffer, '\0', win->buffer_size);

  win_input_buffer_update(win);
}

void win_input_title_set(win_input_t* win, char* title)
{
  win->title = title;

  win->title_len = (title ? strlen(title) : 0);
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 *
 * RETURN (win_input_t* win)
 */
win_input_t* win_input_create(char* name, bool active, bool tab_ability, int x, int y, int w, char* buffer, size_t size, char* title, bool secret, win_event_t* event)
{
  win_input_t* win = malloc(sizeof(win_input_t));

  win->head = win_head_create(WIN_INPUT, name, active, tab_ability, x, y, w, 3, event);

  win_input_buffer_set(win, buffer, size);

  win->hidden = win->secret = secret;

  win->title     = title;
  win->title_len = (title ? strlen(title) : 0);

  return win;
}

/*
 *
 */
void win_input_free(win_input_t* win)
{
  if(win == NULL) return;

  win_head_free(win->head);

  free(win);
}

/*
 *
 */
static void win_input_buffer_print(win_input_t* win)
{
  WINDOW* window = win->head.window;

  if(window == NULL) return;

  int xmax = win->head.xmax;

  wmove(window, 1, 1);

  // The amount of characters to print
  int amount = MIN(win->buffer_len, xmax - 2);

  for(int index = 0; index < amount; index++)
  {
    bool hidden = (win->hidden && win->secret);

    int print_index = win->scroll + index;

    char symbol = (hidden ? '*' : win->buffer[print_index]);

    if(symbol == '\0') symbol = ' ';

    waddch(window, symbol);
  }
}

/*
 *
 */
static void win_input_length_print(win_input_t* win)
{
  WINDOW* window = win->head.window;

  if(window == NULL) return;

  int x = win->head.xmax - 8;

  mvwprintw(window, 2, x, "%03d/%03d", win->buffer_len, win->buffer_size);
}

/*
 * Print the title of the text window
 */
static void win_input_title_print(win_input_t* win)
{
  if(!win->title || !win->title_len) return;

  WINDOW* window = win->head.window;

  if(window == NULL) return;

  int xmax = win->head.xmax;

  int length = MIN(xmax - 2, win->title_len);

  wmove(window, 0, (xmax - length) / 2);

  for(int index = 0; index < length; index++)
  {
    waddch(window, win->title[index]);
  }
}

/*
 * Refresh the content of the buffer being shown
 */
void win_input_refresh(win_input_t* win)
{
  if(!win->head.active) return;

  win_head_clean((win_head_t*) win);

  WINDOW* window = win->head.window;

  if(window == NULL) return;

  box(window, 0, 0);

  win_input_title_print(win);

  win_input_length_print(win);

  if(win->buffer)
  {
    win_input_buffer_print(win);

    wmove(window, 1, 1 + win->cursor - win->scroll);

    curs_set(1);
  }

  wrefresh(window);
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Symbol not supported
 * - 2 | Buffer is full
 */
static int win_input_symbol_add(win_input_t* win, char symbol)
{
  if(symbol < 32 || symbol > 126) return 1;

  if(win->buffer_len >= win->buffer_size) return 2;


  int xmax = win->head.xmax;

  // Shift characters forward to make room for new character
  for(int index = win->buffer_len + 1; index-- > win->cursor;)
  {
    win->buffer[index] = win->buffer[index - 1];
  }

  win->buffer[win->cursor] = symbol;

  win->buffer_len++;

  win->cursor = MIN(win->cursor + 1, win->buffer_len);

  // The cursor is at the end of the input window
  if((win->cursor - win->scroll) >= (xmax - 2))
  {
    win->scroll++; // Scroll one more character
  }

  return 0; // Success!
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | No symbol to delete
 */
static int win_input_symbol_del(win_input_t* win)
{
  if(win->cursor <= 0) return 1;

  // Fill in the room left by the deleted character
  for(int index = win->cursor - 1; index < win->buffer_len; index++)
  {
    win->buffer[index] = win->buffer[index + 1];
  }

  win->buffer_len = MAX(0, win->buffer_len - 1);

  win->buffer[win->buffer_len] = '\0';

  win->cursor = MIN(win->cursor - 1, win->buffer_len);

  return 0; // Success!
}

/*
 *
 */
static void win_input_scroll_right(win_input_t* win)
{
  // The cursor can not be further than the text itself
  win->cursor = MIN(win->buffer_len, win->cursor + 1);

  int xmax = win->head.xmax;

  // The cursor is at the end of the input window
  if((win->cursor - win->scroll) >= (xmax - 2))
  {
    win->scroll++; // Scroll one more character
  }
}

/*
 *
 */
static void win_input_scroll_left(win_input_t* win)
{
  win->cursor = MAX(0, win->cursor - 1);

  // If the cursor is to the left of the window,
  // scroll to the start of the cursor
  if(win->cursor < win->scroll)
  {
    win->scroll = win->cursor;
  }
}

/*
 * Note: If input window has no buffer, nothing should be done
 */
void win_input_event(win_head_t* win_head, int key)
{
  if(win_head == NULL || win_head->type != WIN_INPUT) return;

  win_input_t* win = (win_input_t*) win_head;


  if(!win->buffer) return;

  switch(key)
  {
    case KEY_CTRLH:
      if(win->secret) win->hidden = !win->hidden;
      break;

    case KEY_RIGHT:
      win_input_scroll_right(win);
      break;

    case KEY_LEFT:
      win_input_scroll_left(win);
      break;

    case KEY_BACKSPACE:
      win_input_symbol_del(win);
      break;
    
    default:
      win_input_symbol_add(win, key);
      break;
  }
}

void pop_input_event(win_head_t* win_head, int key)
{
  if(win_head == NULL || win_head->type != WIN_INPUT) return;

  win_input_event(win_head, key);

  if(key == KEY_ENTR) win_head->active = false;
}

win_input_t* wins_name_win_input_get(win_t** wins, int count, char* name)
{
  win_t* win = wins_name_win_get(wins, count, name);

  if(!win || win->type != WIN_INPUT) return NULL;

  return (win_input_t*) win;
}
