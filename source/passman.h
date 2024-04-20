#ifndef PASSMAN_H
#define PASSMAN_H

#include "aes.h"
#include "file.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include <ncurses.h>

typedef struct
{
  char account[64];
  char website[64];
  char email[64];
  char name[64];
  char password[64];
  char cdate[16];
  char udate[16];
} account_t;

typedef struct
{
  char phash[64]; // Password hash (SHA256)
  char email[64];
  char cdate[16];
  char udate[16];
  uint8_t amount;
  account_t accounts[128];
} database_t;

#define DBSIZE     sizeof(database_t)
#define DBSIZE_ENC (DBSIZE + 16 - (DBSIZE % 16))

typedef struct
{
  WINDOW* window;
  int xmax;
  char* buffer;
  int length;
  int cursor;
} inpwin_t;

typedef struct
{
  WINDOW* window;
  int ymax;
  int xmax;
  int index;      // Which database the user hovers over
  char** dbases;  // The names of the available databases
  int amount;     // The amount of databases
} dbswin_t;

#endif // PASSMAN_H
