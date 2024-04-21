#include "../passman.h"

void acswin_key_handler(acswin_t* acswin, int key, inpwin_t* inpwin)
{
  switch(key)
  {
    case 'j':
      acswin->index = MIN(acswin->index + 1, database.amount - 1);
      break;

    case 'k':
      acswin->index = MAX(acswin->index - 1, 0);
      break;

    case 'd':
      move(0, 0);
      printw("delete");
      refresh();
      break;

    case 'n':
      move(0, 0);
      printw("new");
      refresh();
      break;

    case 'c':
      move(0, 0);
      printw("copy");
      refresh();
      break;

    case 's':
      move(0, 0);
      printw("show");
      refresh();
      break;

    case '/':
      move(0, 0);
      printw("search");
      refresh();
      break;

    default:
      break;
  }
}

void menu_database(void)
{
  char text[64];
  inpwin_t* inpwin = inpwin_create(1, 1, 1, text, sizeof(text));

  int ymax = getmaxy(stdscr);

  acswin_t* acswin = acswin_center_create(stdscr, ymax - 10, 7, 5);

  wborder(inpwin->window, 0, 0, 0, 0, 0, 0, ACS_VLINE, ACS_VLINE);
  wborder(acswin->window, 0, 0, 0, 0, ACS_VLINE, ACS_VLINE, 0, 0);

  refresh();

  inpwin_refresh(inpwin, false);
  acswin_refresh(acswin);

  int key;
  while((key = wgetch(acswin->window)))
  {
    if(key == 10) break;


    acswin_refresh(acswin);
  }

  acswin_free(acswin);
  inpwin_free(inpwin);
}
