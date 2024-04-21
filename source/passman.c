#include "passman.h"

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
 * The menu to authenticate a database using password
 */
void menu_password(void)
{
  char password[32];
  memset(password, '\0', sizeof(password));

  string_input(password, sizeof(password), "Password", true);

  refresh();

  printw("%s", password);

  getch();
}

void menu_databases(void)
{
  char text[64];
  inpwin_t* inpwin = inpwin_center_create(stdscr, 5, 5, text, sizeof(text));

  int ymax = getmaxy(stdscr);

  char* dbases[] = {"Secret", "Home", "School"};
  int amount = 3;

  dbswin_t* dbswin = dbswin_center_create(stdscr, ymax - 10, 8, 5, dbases, amount);

  wborder(inpwin->window, 0, 0, 0, 0, 0, 0, ACS_VLINE, ACS_VLINE);
  wborder(dbswin->window, 0, 0, 0, 0, ACS_VLINE, ACS_VLINE, 0, 0);

  refresh();

  inpwin_refresh(inpwin, false);
  dbswin_refresh(dbswin);

  int key;
  while((key = wgetch(dbswin->window)))
  {
    if(key == 10) break;

    switch(key)
    {
      case 'j':
        dbswin->index = MIN(dbswin->index + 1, amount - 1);
        break;

      case 'k':
        dbswin->index = MAX(dbswin->index - 1, 0);
        break;

      case 'd':
        move(0, 0);
        printw("delete");
        refresh();
        bool answer = false;
        confirm_input(&answer, "Delete Database?", "Yes", "No");
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

        char string[64];

        string_input(string, sizeof(string), "Name", false);

        printw("string: %s", string);

        break;

      case '/':
        move(0, 0);
        printw("search");
        refresh();

        inpwin_input(inpwin, false);
        break;

      default:
        break;
    }

    dbswin_refresh(dbswin);
  }

  dbswin_free(dbswin);
  inpwin_free(inpwin);
}

void menu_database(void)
{
  char text[64];
  inpwin_t* inpwin = inpwin_center_create(stdscr, 5, 5, text, sizeof(text));

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

        inpwin_input(inpwin, false);
        break;

      default:
        break;
    }

    acswin_refresh(acswin);
  }

  acswin_free(acswin);
  inpwin_free(inpwin);
}

void menu_account(void)
{
  char string[64];

  string_input(string, sizeof(string), "Name", false);

  printw("string: %s", string);

  getch();
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
