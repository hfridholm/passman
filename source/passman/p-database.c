#include "../passman.h"

void dbase_names_get(char** names, int* count)
{
  char** files = files_
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

  aes_decrypt(&database, buffer, sizeof(buffer), password, AES_256);

  char hash[64];
  sha256(hash, password, strlen(password));

  // If the hashes don't match, the password was wrong
  if(strncmp(hash, database.psw_hash, 64)) return 2;

  return 0; // Success!
}

/*
 * RETURN (int status)
 * - 0 | Success!
 */
int database_write(void)
{
  size_t encrypt_size = sizeof(database_t) + 16 - (sizeof(database_t) % 16))

  char buffer[encrypt_size];
  memset(buffer, '\0', sizeof(buffer));

  char hash[64];
  sha256(hash, password, strlen(password));

  memcpy(database.psw_hash, hash, 64);

  aes_encrypt(buffer, &database, encrypt_size, password, AES_256);

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

