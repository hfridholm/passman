#include "../passman.h"

void menu_password_inpwin_input()
{
  /*
  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  inpwin_t* inpwin = inpwin_create(xmax / 2, ymax / 2, 20, string, size);
  if(inpwin == NULL) return;

  window_title_center_print(inpwin->window, prompt);

  refresh();

  curs_set(1);

  inpwin_refresh(inpwin, hidden);

  int key;
  while((key = wgetch(inpwin->window)))
  {
    if(key == 10) break;

    inpwin_key_handler();
  
    inpwin_refresh(inpwin, hidden);
  }
  curs_set(0);

  inpwin_free(inpwin);
  */
}

void menpsw_refresh(void)
{
  refresh();

  inpwin_refresh(menpsw.pswwin, true);
}

void menpsw_resize(void)
{
  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  inpwin_resize(menpsw.pswwin, xmax / 2, ymax / 2, 50);
}

void menpsw_input(void)
{
  menpsw_refresh();

  int key;
  while((key = wgetch(menpsw.pswwin->window)))
  {
    if(key == 10) break;

    inpwin_key_handler(menpsw.pswwin, key);

    if(key == KEY_RESIZE) menpsw_resize();
  
    menpsw_refresh();
  }
}

void menpsw_init(void)
{
  menpsw.pswwin = inpwin_create(1, 1, 1,
    menpsw.password, sizeof(menpsw.password));

  menpsw_resize();
}

void menpsw_free(void)
{
  inpwin_free(menpsw.pswwin);
}
