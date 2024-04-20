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

void menu_databases(void)
{

}

/*
 * Input text from inside a specified window
 *
 * Don't let the user input more text than specified,
 * or what the window can hold
 */
void window_input(char* buffer, size_t size, WINDOW* window, bool secret)
{
  int ymax = getmaxy(window);
  int xmax = getmaxx(window);

  keypad(window, TRUE);

  int x = 1;
  int y = 1;

  wmove(window, y, x);

  int index = 0;
  char key;

  while((key = wgetch(window)) && index < size)
  {
    if(key == 10) break;

    if(key == 7 && index > 0)
    {
      buffer[--index] = '\0';

      if(x <= 1 && y > 1)
      {
        x = xmax - 2;
        y--;
      }
      x--;
      wmove(window, y, x);
      waddch(window, ' ');
      wmove(window, y, x);
    }

    if(key >= 32 && key <= 126)
    {
      if(x >= xmax - 2)
      {
        if(y >= ymax - 2) continue;

        x = 1;
        y++;

        wmove(window, y, x);
      }
      else x++;

      waddch(window, secret ? '*' : key);

      buffer[index++] = key;
    }
  }
}

/*
 * Input password from a popup window prompting you
 */
void password_window_input(char* password, size_t size)
{
  int xmax = getmaxx(stdscr);

  int width = MIN(32, xmax - 6);

  WINDOW* window = window_center_create(stdscr, 3, width);
  box(window, 0, 0);

  window_title_center_print(window, "Password");

  refresh();
  wrefresh(window);

  window_input(password, size, window, true);
}

/*
 * The menu to authenticate a database using password
 */
void menu_password(void)
{
  char password[32];

  password_window_input(password, sizeof(password));

  printw("%s", password);

  getch();
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

  menu_password();

  endwin();

  return 0; // Success!
}
