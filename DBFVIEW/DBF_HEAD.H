//---------------------------------------------------------------------------

#ifndef DBF_HeadH
#define DBF_HeadH
//---------------------------------------------------------------------------
#endif

typedef struct
{
  char dbf_id;
  char last_update[3];
  long last_rec;
  short unsigned data_offset;
  short unsigned rec_size;
  char filler[20];
}DBF_HEAD;


