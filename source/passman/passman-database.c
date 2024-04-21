#include "../passman.h"

ksize_t encrypt = AES_256;

char dbfile[256];
char pwfile[256];

database_t database;
char       password[64];

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

