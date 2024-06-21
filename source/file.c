/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-06-16
 */

#include "file.h"
#include "passman.h"

/*
 * PARAMS
 * - char**      names   |
 * - size_t*     count   |
 * - const char* dirpath |
 * 
 * RETURN (int status)
 * - 0 | Success!
 * - 1 | Failed to open directory
 */
int dir_file_names(char*** names, size_t* count, const char* dirpath)
{
  struct dirent* dire;

  DIR* dirp = opendir(dirpath);

  if(dirp == NULL) return 1;

  while((dire = readdir(dirp)) != NULL)
  {
    if(!strcmp(dire->d_name, ".")) continue;

    if(!strcmp(dire->d_name, "..")) continue;


    if(dire->d_type != DT_REG) continue;

    char** new_names = realloc(*names, sizeof(char*) * ((*count) + 1));

    if(!new_names)
    {
      printf("realloc failed\n");

      closedir(dirp);

      return 1;
    }
    else *names = new_names;

    (*names)[(*count)] = strdup(dire->d_name);

    (*count)++;
  }

  closedir(dirp);

  return 0; // Success!
}

/*
 * Get the size of the data in a file at the inputted path
 *
 * PARAMS
 * - const char* filepath | The path to the file
 *
 * RETURN (size_t size)
 * - 0  | Error
 * - >0 | Success!
 */
size_t file_size(const char* filepath)
{
  FILE* stream = fopen(filepath, "rb");

  if(stream == NULL) return 0;

  fseek(stream, 0, SEEK_END); 

  size_t size = ftell(stream);

  fseek(stream, 0, SEEK_SET); 

  fclose(stream);

  return size;
}

/*
 * Write data to file and store it at the inputted filepath
 *
 * PARAMS
 * - const void* pointer  | The address to the data to write
 * - size_t size          | The size of the data to write
 * - size_t nmemb         | The size of the chunks
 * - const char* filepath | The path to the file
 *
 * RETURN (same as fwrite)
 * - 0  | Error
 * - >0 | Success!
 */
int file_write(const void* pointer, size_t size, size_t nmemb, const char* filepath)
{
  FILE* stream = fopen(filepath, "wb");

  if(stream == NULL) return 0;

  int status = fwrite(pointer, size, nmemb, stream);

  fclose(stream);

  return status;
}

/*
 * Read data from file and store it at the inputted filepath
 *
 * PARAMS
 * - void* pointer        | The address to store the read data
 * - size_t size          | The size of the data to read
 * - size_t nmemb         | The size of the chunks
 * - const char* filepath | The path to the file
 *
 * RETURN (same as fread)
 * - 0  | Error
 * - >0 | Success!
 */
int file_read(void* pointer, size_t size, size_t nmemb, const char* filepath)
{
  FILE* stream = fopen(filepath, "rb");

  if(stream == NULL) return 0;

  int status = fread(pointer, size, nmemb, stream);

  fclose(stream);

  return status;
}

/*
 * RETURN (same as rename)
 */
int dir_file_rename(const char* dirpath, const char* old_name, const char* new_name)
{
  size_t old_path_size = strlen(dirpath) + 1 + strlen(old_name);

  char old_filepath[old_path_size + 1];

  sprintf(old_filepath, "%s/%s", dirpath, old_name);

  size_t new_path_size = strlen(dirpath) + 1 + strlen(new_name);

  char new_filepath[new_path_size + 1];

  sprintf(new_filepath, "%s/%s", dirpath, new_name);

  return rename(old_filepath, new_filepath);
}

int dir_file_read(void* pointer, size_t size, size_t nmemb, const char* dirpath, const char* name)
{
  size_t path_size = strlen(dirpath) + 1 + strlen(name);

  char filepath[path_size + 1];

  sprintf(filepath, "%s/%s", dirpath, name);

  return file_read(pointer, size, nmemb, filepath);
}

int dir_file_write(const void* pointer, size_t size, size_t nmemb, const char* dirpath, const char* name)
{
  size_t path_size = strlen(dirpath) + 1 + strlen(name);

  char filepath[path_size + 1];

  sprintf(filepath, "%s/%s", dirpath, name);

  return file_write(pointer, size, nmemb, filepath);
}

size_t dir_file_size(const char* dirpath, const char* name)
{
  size_t path_size = strlen(dirpath) + 1 + strlen(name);

  char filepath[path_size + 1];

  sprintf(filepath, "%s/%s", dirpath, name);

  return file_size(filepath);
}
