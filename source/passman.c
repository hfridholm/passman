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

  database.amount = 1;
  database.accounts[0] = (account_t) {
    .account = "google.com"
  };

  screen_init();

  mendbe_input();

  screen_free();

  printf("Exited Successfully!\n");

  return 0; // Success!
}
