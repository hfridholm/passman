#ifndef P_DBASE_H
#define P_DBASE_H

typedef struct
{
  char account[64];
  char website[64];
  char email[64];
  char name[64];
  char password[64];
  char date_create[16];
  char date_update[16];
} accnt_t;

typedef struct
{
  char    name[64];
  char    psw_hash[64];    // Password hash (SHA256)
  char    email[64];
  char    date_create[16];
  char    date_update[16];
  uint8_t accnt_count;
  accnt_t accnts[128];
} dbase_t;

#define DBASE_DIR "../assets/"

extern int dbase_read(dbase_t* dbase, const char* name, const char* password);

extern int dbase_write(dbase_t* dbase, const char* name, const char* password);

#endif // P_DBASE_H
