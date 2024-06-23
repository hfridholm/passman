#include "../passman.h"

/*
 * RETURN (win_list_item_t* item)
 */
static win_list_item_t* win_list_item_get(win_list_t* win)
{
  if(!win || !win->items || !win->show_items) return NULL;

  if(!BETWEEN(win->show_item_index, 0, win->show_item_count - 1)) return NULL;

  return win->show_items[win->show_item_index];
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Bad input
 * - 2 | Failed to get item
 */
int win_list_item_string_set(win_list_t* win, const char* string)
{
  if(!win || !string) return 1;

  win_list_item_t* item = win_list_item_get(win);

  if(!item) return 2;

  strncpy(item->string, string, sizeof(item->string));

  return 0;
}

/*
 * RETURN (char* string)
 * Note: NULL means either bad input, or item missing
 */
char* win_list_item_string_get(win_list_t* win)
{
  win_list_item_t* item = win_list_item_get(win);

  return item ? item->string : NULL;
}

/*
 * RETURN (win_list_item_t item)
 */
static win_list_item_t win_list_item_create(const char* string, const char* preview)
{
  win_list_item_t item;

  if(string)
  {
    strncpy(item.string, string, sizeof(item.string));
  }
  else memset(item.string, '\0', sizeof(item.string));

  if(preview)
  {
    strncpy(item.preview, preview, sizeof(item.preview));
  }
  else memset(item.preview, '\0', sizeof(item.preview));

  return item;
}

/*
 *
 */
static void win_list_index_item_delete(win_list_t* win, int item_index)
{
  for(int index = item_index; index < (win->item_count - 1); index++)
  {
    win->items[index] = win->items[index + 1];
  }

  win->items = realloc(win->items, sizeof(win_list_item_t) * (win->item_count - 1));

  win->item_count--;
}

/*
 * RETURN (int index)
 * Note: -1 means error
 */
static int win_list_item_index_get(win_list_t* win)
{
  char* string = win_list_item_string_get(win);

  if(!string) return -1;

  for(int index = 0; index < win->item_count; index++)
  {
    win_list_item_t item = win->items[index];

    if(strcmp(item.string, string) == 0) return index;
  }
  return -1;
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Bad input
 * - 2 | No item selected
 */
int win_list_item_delete(win_list_t* win)
{
  if(!win || !win->show_items || !win->items) return 1;

  int index = win_list_item_index_get(win);

  if(index == -1) return 2;

  win_list_index_item_delete(win, index);

  win_list_items_filter(win);

  win->show_item_index = MAX(MIN(win->show_item_index, win->show_item_count - 1), 0);

  return 0;
}

/*
 * RETURN (bool result)
 * - true  | The item exists
 * - false | The item doesn't exist, or bad input
 */
bool win_list_item_exists(win_list_t* win)
{
  return win_list_item_get(win) ? true : false;
}

/*
 * RETURN (bool result)
 * - true  | The item with the string exists
 * - false | The item doesn't exist, or bad input
 */
bool win_list_string_item_exists(win_list_t* win, const char* string)
{
  if(!win || !string) return false;

  for(int index = 0; index < win->item_count; index++)
  {
    win_list_item_t item = win->items[index];

    if(strcmp(item.string, string) == 0) return true;
  }
  return false;
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Bad input
 */
int win_list_items_empty(win_list_t* win)
{
  if(!win) return 1;

  if(win->items) free(win->items);

  win->items      = NULL;
  win->item_count = 0;

  if(win->show_items) free(win->show_items);

  win->show_items      = NULL;
  win->show_item_count = win->show_item_index = 0;

  return 0;
}

/*
 *
 */
static void win_list_show_items_empty(win_list_t* win)
{
  if(win->show_items) free(win->show_items);

  win->show_items      = NULL;
  win->show_item_count = win->show_item_index = 0;
}

static void win_list_show_item_add(win_list_t* win, win_list_item_t* item)
{
  win->show_items = realloc(win->show_items, sizeof(win_list_item_t*) * (win->show_item_count + 1));

  win->show_items[win->show_item_count++] = item;
}

/*
 * RETURN (win_list_t* win)
 */
win_list_t* win_list_items_filter(win_list_t* win)
{
  if(!win) return NULL;

  win_list_show_items_empty(win);

  char* filter = win->buffer_filter;
  size_t filter_len = filter ? strlen(filter) : 0;

  for(int index = 0; index < win->item_count; index++)
  {
    win_list_item_t* item = &win->items[index];

    // Only actually filter if the filter exists
    if(filter && strncmp(item->string, filter, filter_len) != 0) continue;

    win_list_show_item_add(win, item);
  }
  
  win_list_items_order(win);

  return win;
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Bad input
 */
int win_list_item_add(win_list_t* win, const char* string, const char* preview)
{
  if(!win || !string) return 1;

  // FIX: Store return of realloc in temp varaible and check it
  win->items = realloc(win->items, sizeof(win_list_item_t) * (win->item_count + 1));

  win->items[win->item_count++] = win_list_item_create(string, preview);

  win_list_items_filter(win);

  return 0;
}

/*
 * EXPECT:
 * - items is an allocated array
 * - index1 and index2 are valid indecies
 *
 * RETURN (bool result)
 * Note: false can be returned on error
 */
static bool win_list_items_swap(win_list_item_t** items, int index1, int index2, bool reverse)
{
  if(!items[index1] || !items[index2]) return false;

  // 1. Compare item strings alphabetically
  int compare = strcmp(items[index1]->string, items[index2]->string);

  if(reverse  && compare >= 0) return false;

  if(!reverse && compare <= 0) return false;


  // 2. Swap items
  win_list_item_t* temp = items[index2];

  items[index2] = items[index1];

  items[index1] = temp;

  return true;
}

/*
 * Credit: https://en.wikipedia.org/wiki/Bubble_sort 
 *
 * RETURN (win_list_t* win)
 */
win_list_t* win_list_items_order(win_list_t* win)
{
  if(!win || !win->items || !win->show_items) return win;

  int count = win->show_item_count;

  while(count > 1)
  {
    int new_count = 0;

    for(int index = 1; index < count; index++)
    {
      win_list_item_t** items = win->show_items;

      if(win_list_items_swap(items, index - 1, index, win->order_reverse))
      {
        new_count = index;
      }
    }
    count = new_count;
  }
  return win;
}
