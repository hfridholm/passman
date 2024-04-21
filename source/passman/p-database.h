#ifndef P_DATABASE_H
#define P_DATABASE_H

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
extern char password[64];

extern ksize_t encrypt;

extern char dbfile[256];
extern char pwfile[256];

#endif // P_DATABASE_H
