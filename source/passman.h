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

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) > (b)) ? (b) : (a))

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


extern database_t database;


typedef WINDOW window_t;

typedef struct
{
  window_t* window;
  int xmax;
  char* buffer;   // The inputted string
  int msize;      // Max size of buffer
  int length;     // The length of the string
  int cursor;     // The index which the cursor is on
  int scroll;     // How many characters that is scrolled
} inpwin_t;       // Input Window

typedef struct
{
  window_t* window;
  int ymax;
  int xmax;
  int index;      // Which database the user hovers over
  char** dbases;  // The names of the available databases
  int amount;     // The amount of databases
  int scroll;     // How many lines has been scrolled down
} dbswin_t;       // Databases Window

typedef struct
{
  window_t* window;
  int ymax;
  int xmax;
  int index;      // Which account the user hovers over
  int scroll;     // How many lines has been scrolled down
} acswin_t;       // Accounts Window

typedef struct
{
  window_t* window;
  int ymax;
  int xmax;
  char* title;    // The title (error, info)
  char* text;     // The text / information
} infwin_t;       // Info Window

typedef struct
{
  window_t* window;
  int ymax;
  int xmax;
  char* prompt;   // The question to answer
  char* ytext;    // The text for answering yes
  char* ntext;    // The text for answering no
  bool answer;    // The answer (yes or no)
} cnfwin_t;       // Confirm Window

extern void string_input(char* string, size_t size, const char* prompt, bool hidden);

extern inpwin_t* inpwin_center_create(window_t* parent, int y, int x, char* buffer, int size);

extern void inpwin_free(inpwin_t* inpwin);

extern void inpwin_input(inpwin_t* inpwin, bool hidden);

extern void inpwin_refresh(inpwin_t* inpwin, bool hidden);

extern void dbswin_free(dbswin_t* dbswin);

extern dbswin_t* dbswin_center_create(window_t* parent, int h, int y, int x, char** dbases, int amount);

extern void dbswin_refresh(dbswin_t* dbswin);

extern void acswin_free(acswin_t* acswin);

extern acswin_t* acswin_center_create(window_t* parent, int h, int y, int x);

extern void acswin_refresh(acswin_t* acswin);

extern void confirm_input(bool* answer, char* prompt, char* ytext, char* ntext);

#endif // PASSMAN_H
