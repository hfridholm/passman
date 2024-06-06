void screen_menu_get(screen_t* screen)
{
  if(screen->menu_index >= 0 && screen->menu_index < screen->menu_count)
  {
    return screen->menus[screen->menu_index];
  }
  return NULL;
}

void screen_menu_add(screen_t* screen, menu_t* menu)
{
  screen->menus = realloc(sizeof(menu_t*) * (screen->menu_count + 1));

  screen->menus[screen->menu_count++] = menu;
}

void screen_menu_dbs_create(screen_t* screen, char* name, int xmax, int ymax)
{
  menu_dbs_t* menu = menu_dbs_create(name, xmax, ymax);

  screen_menu_add(screen, menu);
}

