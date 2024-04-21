#include "passman.h"

bool argnew = false;
bool argopen = false;

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

/*
 * The menu to authenticate a database using password
 */
void menu_password(void)
{
  /*
  char password[32];
  memset(password, '\0', sizeof(password));

  string_input(password, sizeof(password), "Password", true);

  refresh();

  printw("%s", password);

  getch();
  */
}

void menu_databases_resize(dbswin_t* dbswin, inpwin_t* inpwin)
{
  int xmax = getmaxx(stdscr);
  int ymax = getmaxy(stdscr);

  int x = xmax / 2;
  int w = xmax - 12;

  inpwin_resize(inpwin, x, 5, w);

  dbswin_resize(dbswin, x, (ymax / 2) + 2, w, ymax - 10);
}

void menu_databases_refresh(dbswin_t* dbswin, inpwin_t* inpwin)
{
  refresh();

  dbswin_refresh(dbswin);

  inpwin_refresh(inpwin, false);
}

typedef struct
{
  dbswin_t* dbases;
  inpwin_t* search;
  cnfwin_t* delete;
} menu_dbases_t;

void menu_databases_inpwin_input(dbswin_t* dbswin, inpwin_t* inpwin)
{
  curs_set(1);

  menu_databases_refresh(dbswin, inpwin);

  int key;
  while((key = wgetch(inpwin->window)))
  {
    if(key == 10) break;

    inpwin_key_handler(inpwin, key);

    if(key == KEY_RESIZE) menu_databases_resize(dbswin, inpwin);
  
    menu_databases_refresh(dbswin, inpwin);
  }
  curs_set(0);
}

void menu_databases_delete_input(bool* answer, dbswin_t* dbswin, inpwin_t* inpwin)
{
  cnfwin_t* cnfwin = cnfwin_create(1, 1, 1, "Why?", "Yes", "No");

  menu_databases_refresh(dbswin, inpwin);

  int key;
  while((key = wgetch(cnfwin->window)))
  {
    if(key == 10) break;

    cnfwin_key_handler(cnfwin, key);

    if(key == KEY_RESIZE) menu_databases_resize(dbswin, inpwin);
  
    menu_databases_refresh(dbswin, inpwin);
  }

  cnfwin_free(cnfwin);
}

void dbswin_key_handler(dbswin_t* dbswin, int key, inpwin_t* inpwin)
{
  switch(key)
  {
    case 'j':
      dbswin->index = MIN(dbswin->index + 1, dbswin->amount - 1);
      break;

    case 'k':
      dbswin->index = MAX(dbswin->index - 1, 0);
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

      // memcpy(dbfile, dbswin->dbases[dbswin->index],
      //   MIN(sizeof(dbfile), strlen(dbswin->dbases[dbswin->index]));
      break;

    case 'r':
      move(0, 0);
      printw("rename");
      refresh();

      /*
      char string[64];

      string_input(string, sizeof(string), "Name", false);

      printw("string: %s", string);
      */

      break;

    case '/':
      move(0, 0);
      printw("search");
      refresh();

      menu_databases_inpwin_input(dbswin, inpwin);

      break;

    case KEY_RESIZE:
      menu_databases_resize(dbswin, inpwin);
      break;

    default:
      break;
  }
}

void menu_databases(void)
{
  char text[64];
  inpwin_t* inpwin = inpwin_create(1, 1, 1, text, sizeof(text));

  char* dbases[] = {"Secret", "Home", "School"};
  int amount = 3;

  dbswin_t* dbswin = dbswin_create(1, 1, 1, 1, dbases, amount);

  menu_databases_resize(dbswin, inpwin);

  refresh();

  menu_databases_refresh(dbswin, inpwin);

  int key;
  while((key = wgetch(dbswin->window)))
  {
    if(key == 10) break;

    dbswin_key_handler(dbswin, key, inpwin);

    move(1, 0);
    printw("%03d", key);
    refresh();

    menu_databases_refresh(dbswin, inpwin);
  }

  dbswin_free(dbswin);
  inpwin_free(inpwin);
}

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

void menu_account(void)
{
  /*
  char string[64];

  string_input(string, sizeof(string), "Name", false);

  printw("string: %s", string);

  getch();
  */
}

void screen_init(void)
{
  initscr();

  cbreak();

  noecho();

  curs_set(0);
}

void screen_free(void)
{
  endwin();
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
  */

  /*
  database_print();

  if(strlen(password) == 0) password_input();

  database_write();
  */

  screen_init();

  database.amount = 1;
  database.accounts[0] = (account_t) {
    .account = "google.com"
  };

  menu_databases();

  screen_free();

  return 0; // Success!
}
