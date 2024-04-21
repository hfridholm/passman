#include "../passman.h"

void mendbe_accnts_key_handler(int key)
{
  switch(key)
  {
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

void mendbe_input(void)
{

}
