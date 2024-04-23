#include "../passman.h"

void menpsw_refresh(void)
{
  inpwin_refresh(menpsw.pswwin);
}

void menpsw_resize(int xmax, int ymax)
{
  inpwin_resize(menpsw.pswwin, xmax / 2, ymax / 2, 50);
}

void menpsw_input(void)
{
  menu = MENU_PASSWORD;

  inpwin_input(menpsw.pswwin, NULL);
}

void menpsw_init(void)
{
  menpsw.pswwin = inpwin_create(1, 1, 1, password, sizeof(password), true);
}

void menpsw_free(void)
{
  inpwin_free(menpsw.pswwin);
}
