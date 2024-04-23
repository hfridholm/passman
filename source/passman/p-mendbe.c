#include "../passman.h"

void mendbe_accnts_key_handler(int key)
{
  switch(key)
  {
    case 'd':
      move(0, 0);
      printw("delete");
      break;

    case 'n':
      move(0, 0);
      printw("new");
      break;

    case 'c':
      move(0, 0);
      printw("copy");
      break;

    case 's':
      move(0, 0);
      printw("show");
      break;

    case '/':
      move(0, 0);
      printw("search");
      break;

    default:
      break;
  }
}

void mendbe_input(void)
{

}
