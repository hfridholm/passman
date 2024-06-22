#include "../passman.h"

void win_list_item_string_set(win_list_t* win, int index, const char* string)
{
  if(!win || !string || !win->items) return;

  if(index < 0 || index >= win->item_count) return;

  win_list_item_t* item = &win->items[index];

  strncpy(item->string, string, sizeof(item->string));
}

void win_list_item_banner_set(win_list_t* win, int index, const char* banner)
{
  if(!win || !banner || !win->items) return;

  if(index < 0 || index >= win->item_count) return;

  win_list_item_t* item = &win->items[index];

  strncpy(item->banner, banner, sizeof(item->banner));
}

char* win_list_item_string_get(win_list_t* win, int index)
{
  if(!win || !win->items) return NULL;

  if(index < 0 || index >= win->item_count) return NULL;

  return win->items[index].string;
}

void win_list_hover_item_string_set(win_list_t* win, const char* string)
{
  if(!win || !string) return;

  win_list_item_string_set(win, win->item_index, string);
}

void win_list_hover_item_banner_set(win_list_t* win, const char* banner)
{
  if(!win || !banner) return;

  win_list_item_banner_set(win, win->item_index, banner);
}

void win_list_hover_item_set(win_list_t* win, const char* string, const char* banner)
{
  win_list_hover_item_string_set(win, string);

  if(banner)
  {
    win_list_hover_item_banner_set(win, banner);
  }
  else win_list_hover_item_banner_set(win, string);
}

char* win_list_hover_item_string_get(win_list_t* win)
{
  if(!win) return NULL;

  return win_list_item_string_get(win, win->item_index);
}

static win_list_item_t win_list_item_create(const char* string, const char* banner)
{
  win_list_item_t item;

  strncpy(item.string, string, sizeof(item.string));
  
  if(banner)
  {
    strncpy(item.banner, banner, sizeof(item.banner));
  }
  else strncpy(item.banner, string, sizeof(item.banner));

  return item;
}

void win_list_item_add(win_list_t* win, const char* string, const char* banner)
{
  if(!win || !string) return;

  win->items = realloc(win->items, sizeof(win_list_item_t) * (win->item_count + 1));


  win->items[win->item_count] = win_list_item_create(string, banner);

  win->item_index = win->item_count;

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

  win->items = realloc(win->items, sizeof(win_list_item_t) * (win->item_count - 1));

  win->item_count--;

  win->item_index = MAX(MIN(win->item_index, win->item_count - 1), 0);
}

void win_list_hover_item_delete(win_list_t* win)
{
  if(!win) return;

  win_list_item_delete(win, win->item_index);
}

bool win_list_hover_item_exists(win_list_t* win)
{
  if(!win || !win->items) return false;

  return (win->item_index >= 0 && win->item_index < win->item_count);
}

bool win_list_string_item_exists(win_list_t* win, const char* string)
{
  if(!win || !string) return false;

  for(int index = 0; index < win->item_count; index++)
  {
    win_list_item_t item = win->items[index];

    if(strcmp(item.string, string)) return true;
  }
  return false;
}
