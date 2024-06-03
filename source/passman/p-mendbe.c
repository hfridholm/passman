#include "../passman.h"

inpwin_t* nmewin;
inpwin_t* emlwin;
lstwin_t* acswin;

char** acsnms;

int winnum = 0;
const int winamt = 2;

/*
 *
 */
void mendbe_refresh(void)
{
  inpwin_refresh(nmewin);

  inpwin_refresh(emlwin);
}

/*
 *
 */
void mendbe_resize(int xmax, int ymax)
{
  int x = xmax / 2;
  int w = xmax - 12;

  inpwin_resize(nmewin, x, 5, w);

  inpwin_resize(emlwin, x, 9, w);
}

/*
 *
 */
void mendbe_init(int xmax, int ymax)
{
  int x = xmax / 2;
  int w = xmax - 12;

  nmewin = inpwin_create(x, 5, w, dbfile, sizeof(dbfile), "Name", false, true);
  
  emlwin = inpwin_create(x, 9, w, database.email, sizeof(database.email), "Email", false, true);
}

/*
 *
 */
void mendbe_free(void)
{
  inpwin_free(nmewin);

  inpwin_free(emlwin);
}

void mendbe_acswin_key_handler(int key)
{
  switch(key)
  {
    case 'd':
      break;

    case 'n':
      break;

    case 'c':
      break;

    case 's':
      break;

    default:
      break;
  }
}

/*
 *
 */
void mendbe_input(void)
{
  menu = MENU_DATABASE;

  screen_refresh();

  int key;
  while(running && (key = wgetch(stdscr)))
  {
    move(0, 0);
    printw("key: %03d", key);
    refresh();

    screen_key_handler(key);

    switch(winnum)
    {
      case 0:
        inpwin_key_handler(nmewin, key);
        break;

      case 1:
        inpwin_key_handler(emlwin, key);
        break;
    }

    switch(key)
    {
      case KEY_TAB:
        winnum = (winnum + 1) % winamt;
        break;

      // case KEY_BTAB:
      //  winnum = (winnum + winamt - 1) % winamt;
    }

    screen_refresh();
  }
}
