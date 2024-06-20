#include "../passman.h"

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 * - int h | height of window
 */
void win_list_resize(win_list_t* win, int x, int y, int w, int h)
{
  win_head_resize((win_head_t*) win, x, y, w, h);

  // 1. Clamp the marked item to the floor
  if((win->item_index - win->scroll) >= (h - 2))
  {
    win->scroll = (win->item_index - (h - 2) + 1);
  }

  // 2. Clamp the marked item to the ceiling
  win->scroll = MIN(win->scroll, win->item_index);

  // 3. Prevent empty space when items can occupy the space
  if(win->scroll + (h - 2) > win->item_count)
  {
    win->scroll = MAX(0, win->item_count - (h - 2));
  }
}

/*
 * PARAMS
 * - int x | x-value center of window
 * - int y | y-value center of window
 * - int w | width of window
 * - int h | height of window
 *
 * RETURN (win_list_t* win)
 */
win_list_t* win_list_create(char* name, bool active, bool tab_ability, int x, int y, int w, int h, win_event_t* event)
{
  win_list_t* win = malloc(sizeof(win_list_t));

  win->head = win_head_create(WIN_LIST, name, active, tab_ability, x, y, w, h, event);

  win->items      = NULL;
  win->item_count = 0;

  return win;
}

void win_list_free(win_list_t* win)
{
  if(win == NULL) return;

  win_head_free(win->head);

  if(win->items) free(win->items);

  free(win);
}

void win_list_refresh(win_list_t* win)
{
  if(!win->head.active) return;

  win_head_clean((win_head_t*) win);

  WINDOW* window = win->head.window;

  if(window == NULL) return;

  int ymax = win->head.ymax;

  int amount = MIN(win->item_count, ymax - 2);

  for(int index = 0; index < amount; index++)
  {
    int print_index = index + win->scroll;

    if(print_index == win->item_index) wattron(window, A_REVERSE);

    mvwprintw(window, index + 1, 1, "%s", win->items[print_index]);

    wattroff(window, A_REVERSE);
  }

  box(window, 0, 0);

  wrefresh(window);
}

static void win_list_scroll_down(win_list_t* win)
{
  win->item_index = MIN(win->item_count - 1, win->item_index + 1);

  int ymax = win->head.ymax;

  if((win->item_index - win->scroll) >= (ymax - 2))
  {
    win->scroll++; // Scroll down one more
  }
}

static void win_list_scroll_up(win_list_t* win)
{
  win->item_index = MAX(0, win->item_index - 1);

  win->scroll = MIN(win->scroll, win->item_index);
}

void win_list_event(win_head_t* win_head, int key)
{
  if(win_head == NULL || win_head->type != WIN_LIST) return;

  win_list_t* win = (win_list_t*) win_head;

  switch(key)
  {
    case 'j':
      win_list_scroll_down(win);
      break;

    case 'k':
      win_list_scroll_up(win);
      break;
  }
}

char* win_list_item_get(win_list_t* win)
{
  if(!win || !win->items) return NULL;

  if(win->item_index < 0 || win->item_index >= win->item_count) return NULL;

  return win->items[win->item_index];
}

win_list_t* wins_name_win_list_get(win_t** wins, int count, char* name)
{
  win_t* win = wins_name_win_get(wins, count, name);

  if(!win || win->type != WIN_LIST) return NULL;

  return (win_list_t*) win;
}

void win_list_item_add(win_list_t* win, char* item)
{
  if(!win || !item) return;

  win->items = realloc(win->items, sizeof(char*) * (win->item_count + 1));

  win->items[win->item_count] = item;

  win->item_count++;
}

void win_list_item_delete(win_list_t* win, int item_index)
{
  if(!win || !win->items) return;

  if(item_index < 0 || item_index >= win->item_count) return;

  for(int index = item_index; index < (win->item_count - 1); index++)
  {
    win->items[index] = win->items[index + 1];
  }

  win->items = realloc(win->items, sizeof(char*) * (win->item_count - 1));

  win->item_count--;
}
