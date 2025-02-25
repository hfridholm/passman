#include "../passman.h"

int dbase_file_rename(const char* old_name, const char* new_name)
{
  return dir_file_rename(DBASE_DIR, old_name, new_name);
}

int dbase_file_remove(const char* name)
{
  return dir_file_remove(DBASE_DIR, name);
}

int dbase_file_read(void* pointer, size_t size, const char* name)
{
  return dir_file_read(pointer, size, DBASE_DIR, name);
}

int dbase_file_write(const void* pointer, size_t size, const char* name)
{
  return dir_file_write(pointer, size, DBASE_DIR, name);
}

size_t dbase_file_size_get(const char* name)
{
  return dir_file_size_get(DBASE_DIR, name);
}

/*
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Database file is empty
 * - 2 | Wrong password
 */
int dbase_load(dbase_t* dbase, const char* name, const char* password)
{
  if(!dbase || !name || !password) return 1;

  size_t size = dbase_file_size_get(name);

  if(size != DBASE_ENCRYPT_SIZE) return 1;

  char buffer[DBASE_ENCRYPT_SIZE];
  memset(buffer, '\0', sizeof(buffer));

  dbase_file_read(buffer, size, name);

  // First, decrypt file to string. Then compare just psw hash
  uint8_t* decrypt;
  size_t   decrypt_size;

  if (aes_decrypt(&decrypt, &decrypt_size, buffer, sizeof(buffer), password, AES_256) != 0)
  {
    return 3;
  }

  char hash[64];
  sha256(hash, password, strlen(password));

  // If the hashes don't match, the password was wrong
  if (memcmp(hash, decrypt, sizeof(uint8_t) * 64) != 0)
  {
    free(decrypt);

    return 4;
  }

  memcpy(dbase, decrypt, sizeof(dbase_t));

  free(decrypt);

  return 0;
}

/*
 * RETURN (int status)
 * - 0 | Success!
 */
int dbase_save(dbase_t* dbase, const char* name, const char* password)
{
  if (!dbase || !name || !password)
  {
    return 1;
  }

  uint8_t* buffer;
  size_t   size;

  char hash[64];
  sha256(hash, password, strlen(password));

  memcpy(dbase->psw_hash, hash, 64);

  if (aes_encrypt(&buffer, &size, dbase, sizeof(dbase_t), password, AES_256) != 0)
  {
    return 2;
  }

  dbase_file_write(buffer, size, name);

  free(buffer);

  return 0;
}

static void dbase_accnt_delete(dbase_t* dbase, int accnt_index)
{
  if(!dbase) return;

  if(accnt_index < 0 || accnt_index >= dbase->accnt_count) return;

  for(int index = accnt_index; index < (dbase->accnt_count - 1); index++)
  {
    dbase->accnts[index] = dbase->accnts[index + 1];
  }

  dbase->accnt_count--;

  dbase->accnts[dbase->accnt_count] = (accnt_t) {0};
}

int dbase_name_accnt_index_get(dbase_t* dbase, const char* name)
{
  if(!dbase || !name) return -1;

  for(int index = 0; index < dbase->accnt_count; index++)
  {
    accnt_t accnt = dbase->accnts[index];

    if(strcmp(name, accnt.name) == 0) return index;
  }

  return -1;
}

void dbase_name_accnt_delete(dbase_t* dbase, const char* name)
{
  int index = dbase_name_accnt_index_get(dbase, name);

  if(index != -1) dbase_accnt_delete(dbase, index);
}
