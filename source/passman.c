#include "passman.h"

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Failed to input string
 * - 2 | Failed to parse string
 */
int stream_string(char* string, FILE* stream)
{
  fflush(stream);

  char buffer[1024];

  if(fgets(buffer, sizeof(buffer), stream) == NULL) return 1;

  return (sscanf(buffer, "%[^\n]%*c", string) == 1) ? 0 : 2;
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Failed to parse arguments
 */
int main(int argc, char* argv[])
{
  screen_t* screen = screen_create();

  screen->menu_index = 0;

  screen_refresh(screen);

  int key;
  while(screen->running && (key = wgetch(stdscr)))
  {
    clear();

    mvprintw(0, 0, "ENTER: %d key: %d", KEY_ENTR, key);

    screen_event(screen, key);

    screen_refresh(screen);
  }

  screen_free(screen);

  printf("Exited Successfully!\n");

  return 0; // Success!
}
