#include "passman.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) > (b)) ? (b) : (a))

ksize_t encrypt = AES_256;

char dbfile[256];
char pwfile[256];

bool argnew = false;
bool argopen = false;

database_t database;
char password[64];

/*
 * Output text when something is wrong with an option
 */
void opt_wrong(void)
{
  if(optopt == 'o' || optopt == 'n' || optopt == 'p')
    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
  else if(isprint(optopt))
    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
  else
    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
}

void opt_open(void)
{
  size_t size = MIN(strlen(optarg), sizeof(dbfile));

  memset(dbfile, '\0', sizeof(dbfile));
  strncpy(dbfile, optarg, size);

  argopen = true;
}

void opt_new(void)
{
  size_t size = MIN(strlen(optarg), sizeof(dbfile));

  memset(dbfile, '\0', sizeof(dbfile));
  strncpy(dbfile, optarg, size);

  argnew = true;
}

void opt_pwfile(void)
{
  size_t size = MIN(strlen(optarg), sizeof(pwfile));

  memset(pwfile, '\0', sizeof(pwfile));
  strncpy(pwfile, optarg, size);
}

/*
 * PARAMS
 * - int opt | The option to parse
 *
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Something wrong with option
 */
int opt_parse(int opt)
{
  switch(opt)
  {
    case 'o': opt_open(); break;

    case 'n': opt_new(); break;

    case 'p': opt_pwfile(); break;

    case '?': opt_wrong(); return 1;

    default : abort();
  }
  return 0; // Success!
}

/*
 * PARAMS (same as main)
 * - int argc     | The amount of arguments
 * - char* argv[] | The array of arguments
 *
 * RETURN ()
 * - 0 | Success!
 * - 1 | Failed to parse option
 */
int args_parse(int argc, char* argv[])
{
  opterr = 0;

  int opt;
  while((opt = getopt(argc, argv, "o:n:p:")) != -1)
  {
    if(opt_parse(opt) != 0)
    {
      fprintf(stderr, "Failed to parse option\n");

      return 1;
    }
  }
  return 0; // Success!
}

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
 * - 1 | Nothing was inputted
 */
int dbfile_input(void)
{
  char buffer[1024];
  memset(buffer, '\0', sizeof(buffer));

  printf("Database: ");

  if(stream_string(buffer, stdin))
  {
    printf("No database was inputted\n");

    return 1;
  }

  memcpy(dbfile, buffer, sizeof(dbfile));

  return 0; // Success!
}

int password_input(void)
{
  char* buffer = getpass("Password: ");

  if(buffer == NULL)
  {
    printf("No password was inputted\n");

    return 1;
  }

  memset(password, '\0', sizeof(password));
  memcpy(password, buffer, strlen(buffer));

  free(buffer);

  return 0; // Success!
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Database file is empty
 * - 2 | Wrong password
 */
int database_read(void)
{
  size_t size = file_size(dbfile);
  if(size == 0) return 1;

  char buffer[size];
  memset(buffer, '\0', sizeof(buffer));

  file_read(buffer, size, sizeof(char), dbfile);

  aes_decrypt(&database, buffer, sizeof(buffer), password, encrypt);

  char hash[64];
  sha256(hash, password, strlen(password));

  // If the hashes don't match, the password was wrong
  if(strncmp(hash, database.phash, 64)) return 2;

  return 0; // Success!
}

/*
 * RETURN (int status)
 * - 0 | Success!
 */
int database_write(void)
{
  char buffer[DBSIZE_ENC]; // Database encrypted size
  memset(buffer, '\0', sizeof(buffer));

  char hash[64];
  sha256(hash, password, strlen(password));

  memcpy(database.phash, hash, 64);

  aes_encrypt(buffer, &database, DBSIZE, password, encrypt);

  file_write(buffer, sizeof(buffer), sizeof(char), dbfile);

  return 0; // Success!
}

int database_read_log(void)
{
  int status = database_read();

  switch(status)
  {
    case 1: printf("Database is empty.\n"); break;

    case 2: printf("Password is wrong.\n"); break;
  }
  return status;
}

void database_print(void)
{
  printf("Created : %.16s\n", database.cdate);
  printf("Updated : %.16s\n", database.udate);

  printf("Email   : %.64s\n", database.email);

  for(uint8_t index = 0; index < database.amount; index++)
  {
    printf("- %.64s\n", database.accounts[index].account);
  }
}

/*
 * RETURN (same as file_read)
 * - 0  | Error
 * - >0 | Success!
 */
int pwfile_read(void)
{
  size_t fsize = file_size(pwfile);
  if(fsize == 0) return 0;

  size_t size = MIN(fsize, sizeof(password));

  return file_read(password, size, sizeof(char), pwfile);
}

/*
 * Create a window in the center of the screen
 */
WINDOW* window_center_create(WINDOW* parent, int height, int width)
{
  int ymax, xmax;
  getmaxyx(parent, ymax, xmax);

  int y = (ymax - height) / 2 - 1;
  int x = (xmax - width) / 2 - 1;

  return newwin(height, width, y, x);
}

/*
 * Print a title in the center top of a window
 */
void window_title_center_print(WINDOW* window, const char* title)
{
  int xmax = getmaxx(window);

  size_t length = strlen(title);

  int x = (xmax - length) / 2 - 1;

  mvwprintw(window, 0, x, "%s", title);
}

void window_inside_clear(WINDOW* window)
{
  int ymax = getmaxy(window);
  int xmax = getmaxx(window);

  for(int y = 1; y < ymax - 1; y++)
  {
    wmove(window, y, 1);

    for(int x = 1; x < xmax - 1; x++)
    {
      waddch(window, ' ');
    }
  }
}

typedef struct
{
  WINDOW* window;
  int xmax;
  char* buffer;
  int length;
  int cursor;
} inpwin_t;

/*
 * Refresh the content of the buffer being shown
 */
void inpwin_refresh(inpwin_t* inpwin, bool hidden)
{
  wmove(inpwin->window, 1, 1);

  for(int index = 0; index < inpwin->length; index++)
  {
    if(index > inpwin->xmax - 2) break;

    char symbol = (hidden ? '*' : inpwin->buffer[index]);

    if(symbol == '\0') symbol = ' ';

    waddch(inpwin->window, symbol);
  }

  wmove(inpwin->window, 1, 1 + inpwin->cursor);

  wrefresh(inpwin->window);
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Symbol not supported
 * - 2 | Buffer is full
 */
int inpwin_symbol_add(inpwin_t* inpwin, char symbol)
{
  if(symbol < 32 || symbol > 126) return 1;

  if(inpwin->length >= inpwin->xmax - 2) return 2;

  for(int index = inpwin->length + 1; index-- > inpwin->cursor;)
  {
    inpwin->buffer[index] = inpwin->buffer[index - 1];
  }

  inpwin->buffer[inpwin->cursor++] = symbol;

  inpwin->length++;

  return 0; // Success!
}

int inpwin_symbol_del(inpwin_t* inpwin)
{
  if(inpwin->cursor <= 0) return 1;

  for(int index = inpwin->cursor; index < inpwin->length; index++)
  {
    inpwin->buffer[index] = inpwin->buffer[index + 1];
  }

  wmove(inpwin->window, 1, inpwin->cursor);
  waddch(inpwin->window, ' ');

  inpwin->buffer[--inpwin->cursor] = '\0';

  inpwin->length--;

  return 0; // Success!
}

/*
 * Input text from inside a specified window
 *
 * Don't let the user input more text than specified,
 * or what the window can hold
 */
void inpwin_input(inpwin_t* inpwin, bool hidden)
{
  curs_set(1);

  inpwin_refresh(inpwin, hidden);

  int key;
  while((key = wgetch(inpwin->window)))
  {
    if(key == 10) break;

    switch(key)
    {
      case KEY_RIGHT:
        inpwin->cursor = MIN(inpwin->cursor + 1, inpwin->length);
        break;

      case KEY_LEFT:
        inpwin->cursor = MAX(inpwin->cursor - 1, 0);
        break;

      case KEY_BACKSPACE:
        inpwin_symbol_del(inpwin);
        break;
      
      default:
        inpwin_symbol_add(inpwin, key);
        break;
    }
  
    inpwin_refresh(inpwin, hidden);
  }
  curs_set(0);
}

inpwin_t* inpwin_create(int h, int w, int y, int x)
{
  inpwin_t* inpwin = malloc(sizeof(inpwin_t));

  inpwin->window = newwin(h, w, y, x);

  inpwin->xmax = getmaxx(inpwin->window);

  keypad(inpwin->window, TRUE);

  inpwin->buffer = malloc(sizeof(char) * inpwin->xmax - 1);
  memset(inpwin->buffer, '\0', inpwin->xmax - 1);

  box(inpwin->window, 0, 0);

  return inpwin;
}

void inpwin_free(inpwin_t* inpwin)
{
  wclear(inpwin->window);

  wrefresh(inpwin->window);

  delwin(inpwin->window);

  free(inpwin->buffer);

  free(inpwin);
}

/*
 * Input password from a popup window prompting you
 */
void password_window_input(char* password, size_t size)
{
  // int xmax = getmaxx(stdscr);

  // int width = MIN(32, xmax - 6);

  inpwin_t* inpwin = inpwin_create(3, 25, 3, 3);

  window_title_center_print(inpwin->window, "Password");

  refresh();
  wrefresh(inpwin->window);

  inpwin_input(inpwin, true);

  memcpy(password, inpwin->buffer, MIN(inpwin->length, size));

  inpwin_free(inpwin);
}

/*
 * The menu to authenticate a database using password
 */
void menu_password(void)
{
  char password[32];
  memset(password, '\0', sizeof(password));

  password_window_input(password, sizeof(password));

  refresh();

  printw("%s", password);

  getch();
}

WINDOW* window_edges_create(WINDOW* parent, int top, int bottom, int left, int right)
{
  int ymax = getmaxy(parent);
  int xmax = getmaxx(parent);

  int height  = ymax - top - bottom;
  int width = xmax - left - right;

  if(height < 0 || width < 0) return NULL;

  return newwin(height, width, top, left);
}

void window_databases_print(WINDOW* window, char* names[], int amount, int hover)
{
  for(int index = 0; index < amount; index++)
  {
    if(index == hover) wattron(window, A_REVERSE);

    mvwprintw(window, index + 1, 1, "%s", names[index]);

    wattroff(window, A_REVERSE);
  }
  wrefresh(window);
}

void menu_databases(void)
{
  inpwin_t* textwin = inpwin_create(3, 20, 5, 5);

  WINDOW* window = window_edges_create(stdscr, 10, 5, 5, 5);
  box(window, 0, 0);

  keypad(window, TRUE);

  refresh();
  wrefresh(window);

  inpwin_refresh(textwin, false);

  char* names[] = {"Bob", "Alice", "Clara"};
  int amount = 3;

  int input = 0;
  int hover = 0;

  do
  {
    if(input == 10) break;

    move(0, 0);
    printw("input: %04d", input);
    refresh();

    switch(input)
    {
      case KEY_DOWN:
        hover = MIN(hover + 1, amount - 1);
        break;

      case KEY_UP:
        hover = MAX(hover - 1, 0);
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

      case 'o':
        move(0, 0);
        printw("open");
        refresh();
        break;

      case 'r':
        move(0, 0);
        printw("rename");
        refresh();
        break;

      case 9:
        move(0, 0);
        printw("tab");
        refresh();

        inpwin_input(textwin, false);
        break;

      default:
        break;
    }

    window_databases_print(window, names, amount, hover);
  }
  while((input = wgetch(window)));

  delwin(window);
  inpwin_free(textwin);
}

void menu_database(void)
{

}

void menu_account(void)
{

}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Failed to parse arguments
 */
int main(int argc, char* argv[])
{
  if(args_parse(argc, argv) != 0) return 1;

  /*
  if(strlen(dbfile) == 0) dbfile_input();

  if(strlen(pwfile) > 0) pwfile_read();

  if(argnew)
  {

  }
  else if(argopen)
  {
    if(strlen(password) == 0) password_input();

    if(database_read_log() != 0) return 2;
  }

  database_print();

  if(strlen(password) == 0) password_input();

  database_write();
  */

  initscr();
  cbreak();
  noecho();
  curs_set(0);

  menu_databases();

  endwin();

  return 0; // Success!
}
