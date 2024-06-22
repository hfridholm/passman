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
  char    psw_hash[64];    // Password hash (SHA256)
  char    name[64];
  char    email[64];
  char    date_create[16];
  char    date_update[16];
  uint8_t accnt_count;
  accnt_t accnts[128];
} dbase_t;

#define DBASE_DIR "../assets/"

#define DBASE_ENCRYPT_SIZE (sizeof(dbase_t) + 16 - (sizeof(dbase_t) % 16))

#define DBASE_ACCNTS_SIZE (sizeof(((dbase_t*)0)->accnts) / sizeof(accnt_t))

extern int dbase_read(dbase_t* dbase, const char* name, const char* password);

extern int dbase_write(dbase_t* dbase, const char* name, const char* password);


extern int    dbase_file_remove(const char* name);

extern int    dbase_file_rename(const char* old_name, const char* new_name);

extern size_t dbase_file_size(const char* name);


extern void dbase_name_accnt_delete(dbase_t* dbase, const char* name);

#endif // P_DBASE_H
