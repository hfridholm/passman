#include "../passman.h"

inpwin_t* pswwin;

void menpsw_refresh(void)
{
  inpwin_refresh(pswwin);
}

void menpsw_resize(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;
  int w = 50;

  inpwin_resize(pswwin, x, y, w);
}

void menpsw_input(void)
{
  menu = MENU_PASSWORD;

  inpwin_input(pswwin, NULL);
}

void menpsw_init(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;
  int w = 50;

  pswwin = inpwin_create(x, y, w, password, sizeof(password), true);
}

void menpsw_free(void)
{
  inpwin_free(pswwin);
}
