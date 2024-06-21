#include "../passman.h"

int dbase_file_rename(const char* old_name, const char* new_name)
{
  return dir_file_rename(DBASE_DIR, old_name, new_name);
}

int dbase_file_read(void* pointer, size_t size, size_t nmemb, const char* name)
{
  return dir_file_read(pointer, size, nmemb, DBASE_DIR, name);
}

int dbase_file_write(const void* pointer, size_t size, size_t nmemb, const char* name)
{
  return dir_file_write(pointer, size, nmemb, DBASE_DIR, name);
}

size_t dbase_file_size(const char* name)
{
  return dir_file_size(DBASE_DIR, name);
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Database file is empty
 * - 2 | Wrong password
 */
int dbase_read(dbase_t* dbase, const char* name, const char* password)
{
  size_t size = dbase_file_size(name);

  if(size != DBASE_ENCRYPT_SIZE) return 1;

  char buffer[DBASE_ENCRYPT_SIZE];
  memset(buffer, '\0', sizeof(buffer));

  dbase_file_read(buffer, size, sizeof(char), name);

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
  char buffer[DBASE_ENCRYPT_SIZE];
  memset(buffer, '\0', sizeof(buffer));

  char hash[64];
  sha256(hash, password, strlen(password));

  memcpy(dbase->psw_hash, hash, 64);

  aes_encrypt(buffer, dbase, sizeof(buffer), password, AES_256);

  dbase_file_write(buffer, sizeof(buffer), sizeof(char), name);

  return 0; // Success!
}
