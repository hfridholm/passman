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

  inpwin_resize(pswwin, x, y, xmax - 10);
}

void menpsw_init(int xmax, int ymax)
{
  int x = xmax / 2;
  int y = ymax / 2;

  pswwin = inpwin_create(x, y, xmax - 10, password, sizeof(password), false, true);
}

void menpsw_free(void)
{
  inpwin_free(pswwin);
}

void menpsw_input(void)
{
  menu = MENU_PASSWORD;

  inpwin_input(pswwin, NULL);
}
