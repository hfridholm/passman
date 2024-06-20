/*
 * Last updated: 2024-06-16
 */

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

extern int file_read(void* pointer, size_t size, size_t nmemb, const char* filepath);


extern size_t dir_file_size(const char* dirpath, const char* name);

extern int    dir_file_write(const void* pointer, size_t size, size_t nmemb, const char* dirpath, const char* name);

extern int    dir_file_read(void* pointer, size_t size, size_t nmemb, const char* dirpath, const char* name);


extern void array_free(void** array, size_t count);

#endif // FILE_H
