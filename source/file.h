/*
 * Last updated: 2024-04-20
 */

#ifndef FILE_H
#define FILE_H

#include <stdio.h>

extern size_t file_size(const char* filepath);

extern int file_read(void* pointer, size_t size, size_t nmemb, const char* filepath);

extern int file_write(const void* pointer, size_t size, size_t nmemb, const char* filepath);

#endif // FILE_H
