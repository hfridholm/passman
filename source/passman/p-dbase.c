#include "../passman.h"

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Database file is empty
 * - 2 | Wrong password
 */
int dbase_read(dbase_t* dbase, const char* name, const char* password)
{
  size_t size = dir_file_size(DBASE_DIR, name);

  if(size == 0) return 1;

  char buffer[size];
  memset(buffer, '\0', sizeof(buffer));

  dir_file_read(buffer, size, sizeof(char), DBASE_DIR, name);

  aes_decrypt(dbase, buffer, sizeof(buffer), password, AES_256);

  char hash[64];
  sha256(hash, password, strlen(password));

  // If the hashes don't match, the password was wrong
  if(strncmp(hash, dbase->psw_hash, 64)) return 2;

  return 0; // Success!
}

/*
 * RETURN (int status)
 * - 0 | Success!
 */
int dbase_write(dbase_t* dbase, const char* name, const char* password)
{
  size_t encrypt_size = sizeof(dbase_t) + 16 - (sizeof(dbase_t) % 16);

  char buffer[encrypt_size];
  memset(buffer, '\0', sizeof(buffer));

  char hash[64];
  sha256(hash, password, strlen(password));

  memcpy(dbase->psw_hash, hash, 64);

  aes_encrypt(buffer, dbase, encrypt_size, password, AES_256);

  dir_file_write(buffer, sizeof(buffer), sizeof(char), DBASE_DIR, name);

  return 0; // Success!
}
