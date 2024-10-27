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
  if((win->show_item_index - win->scroll) >= (h - 2))
  {
    win->scroll = (win->show_item_index - (h - 2) + 1);
  }

  // 2. Clamp the marked item to the ceiling
  win->scroll = MIN(win->scroll, win->show_item_index);

  // 3. Prevent empty space when items can occupy the space
  if(win->scroll + (h - 2) > win->show_item_count)
  {
    win->scroll = MAX(0, win->show_item_count - (h - 2));
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
win_list_t* win_list_create(char* name, bool active, bool tab_ability, int x, int y, int w, int h, int max_item_count, char* buffer_filter, win_event_t* event)
{
  win_list_t* win = malloc(sizeof(win_list_t));

  win->head = win_head_create(WIN_LIST, name, active, tab_ability, x, y, w, h, event);

  win->items      = NULL;
  win->item_count = 0;

  win->max_item_count = max_item_count;

  win->show_items      = NULL;
  win->show_item_count = win->show_item_index = 0;

  win->buffer_filter = buffer_filter;

  win->order_reverse = false;

  return win;
}

void win_list_free(win_list_t* win)
{
  if(!win) return;

  win_head_free(win->head);

  if(win->items) free(win->items);

  if(win->show_items) free(win->show_items);

  free(win);
}

/*
 * EXPECT:
 * - win is an allocated win_list_t
 */
static void win_list_count_print(win_list_t* win)
{
  WINDOW* window = win->head.window;

  int y = win->head.h - 1;

  if(win->show_item_count > 0)
  {
    int x = win->head.w - 8;

    mvwprintw(window, y, x, "%03d/%03d", win->show_item_index + 1, win->show_item_count);
  }
}

void win_list_refresh(win_list_t* win)
{
  if(!win || !win->head.window) return;

  WINDOW* window = win->head.window;

  int show_amount = MIN(win->show_item_count, win->head.h - 2);

  for(int index = 0; index < show_amount; index++)
  {
    int print_index = index + win->scroll;

    if(print_index == win->show_item_index)
    {
      wattron(window, A_REVERSE);
    }

    win_list_item_t* item = win->show_items[print_index];

    mvwprintw(window, index + 1, 1, "#%02d: %s", print_index + 1, item->string);

    wattroff(window, A_REVERSE);
  }

  win_border_print((win_t*) win);

  win_list_count_print(win);

  wrefresh(window);
}

static void win_list_scroll_down(win_list_t* win)
{
  // 1. Update selected item (increase item index)
  win->show_item_index = MIN(win->show_item_count - 1, win->show_item_index + 1);

  int visable_items = win->head.h - 2;

  // 2. Constrain scroll to the floor
  win->scroll = MAX(win->scroll, win->show_item_index + 1 - visable_items);
}

static void win_list_scroll_up(win_list_t* win)
{
  // 1. Update selected item (decrease item index)
  win->show_item_index = MAX(0, win->show_item_index - 1);

  // 2. Constrain scroll to the ceiling
  win->scroll = MIN(win->scroll, win->show_item_index);
}

int win_list_event(win_head_t* win_head, int key)
{
  if(!win_head || win_head->type != WIN_LIST) return 0;

  switch(key)
  {
    case 'j': case 'J': case KEY_DOWN:
      win_list_scroll_down((win_list_t*) win_head);
      return 1;

    case 'k': case 'K': case KEY_UP:
      win_list_scroll_up((win_list_t*) win_head);
      return 2;

    default:
      return 0;
  }
}
