win_t* screen_pop_get(screen_t* screen)
{
  if(screen->pop_index >= 0 && screen->pop_index < screen->pop_count)
  {
    return screen->pops[screen->pop_index];
  }
  return NULL;
}

void screen_pop_add(screen_t* screen, win_t* pop)
{
  screen->pops = realloc(sizeof(win_t*) * (screen->pop_count + 1));

  screen->pops[screen->pop_count++] = pop;
}

void screen_pop_confirm_create(screen_t* screen, char* name, int x, int y, int w, char* prompt, char* text_yes, char* text_no, bool active, key_handler_t* key_handler)
{
  win_t* pop = win_confirm_create(name, x, y, w, prompt, text_yes, text_no, active, key_handler);

  screen_pop_add(screen, pop);
}

int screen_pop_confirm_resize(screen_t* screen, char* name, int x, int y, int w)
{
  win_t* pop = wins_name_win_get(screen->pops, screen->pop_count, name);

  if(pop == NULL) return 1;

  if(pop->type != WIN_CONFIRM) return 2;

  win_confirm_resize(pop, x, y, w);

  return 0;
}
