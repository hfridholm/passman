#include "../passman.h"

void delpop_input(void)
{
  popup = POPUP_DELETE;

  screen_refresh();

  int key;
  while(running && (key = wgetch(delpop->window)))
  {
    if(key == 10) break;

    screen_key_handler(key);

    cnfwin_key_handler(delpop, key);

    screen_refresh();
  }

  popup = POPUP_NONE;
}

void pswpop_input(void)
{
  popup = POPUP_OPEN;
  
  screen_refresh();

  int key;
  while(running && (key = wgetch(pswpop->window)))
  {
    if(key == 10) break;

    screen_key_handler(key);

    inpwin_key_handler(pswpop, key);

    screen_refresh();
  }

  popup = POPUP_NONE;
}

void extpop_input(void)
{
  popup = POPUP_EXIT;

  screen_refresh();

  int key;
  while(running && (key = wgetch(extpop->window)))
  {
    if(key == 10) break;

    screen_key_handler(key);

    cnfwin_key_handler(extpop, key);

    screen_refresh();
  }

  if(extpop->answer == true) running = false;

  popup = POPUP_NONE;
}
