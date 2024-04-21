#include "../passman.h"

void menpsw_refresh(void)
{
  inpwin_refresh(menpsw.pswwin, true);
}

void menpsw_resize(int xmax, int ymax)
{
  inpwin_resize(menpsw.pswwin, xmax / 2, ymax / 2, 50);
}

void menpsw_input(void)
{
  menu = MENU_PASSWORD;

  screen_refresh();

  int key;
  while(running && (key = wgetch(menpsw.pswwin->window)))
  {
    if(key == KEY_ENTR) break;

    screen_key_handler(key);

    inpwin_key_handler(menpsw.pswwin, key);

    screen_refresh();
  }
}

void menpsw_init(void)
{
  menpsw.pswwin = inpwin_create(1, 1, 1, password, sizeof(password));
}

void menpsw_free(void)
{
  inpwin_free(menpsw.pswwin);
}
