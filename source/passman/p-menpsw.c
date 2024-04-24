#include "../passman.h"

inpwin_t* pswwin;

void menpsw_refresh(void)
{
  inpwin_refresh(pswwin);
}

void menpsw_resize(int xmax, int ymax)
{
  inpwin_resize(pswwin, xmax / 2, ymax / 2, 50);
}

void menpsw_input(void)
{
  menu = MENU_PASSWORD;

  inpwin_input(pswwin, NULL);
}

void menpsw_init(void)
{
  pswwin = inpwin_create(1, 1, 1, password, sizeof(password), true);
}

void menpsw_free(void)
{
  inpwin_free(pswwin);
}
