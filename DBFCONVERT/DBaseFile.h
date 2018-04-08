//---------------------------------------------------------------------------
#ifndef DBaseFileH
#define DBaseFileH

typedef struct
{
  char dbf_id;
  char last_update[3];
  long last_rec;
  short unsigned data_offset;
  char unsigned rec_size;
  char filler[20];
} DBF_HEAD;

typedef struct
{
  char field_name[11];
  char field_type;
  char dummy[4];
  union
  {
    short unsigned char_len;
    struct
    {
      char len;
      char dec;
    } num_size;
  } len_info;
  char filler[14];
} FIELD_REC;

class DBaseFile
{
  private:
    static const int _HeaderSize = 32;
    static const int _SizePerField = 32;

    // Header
    bool _memo;                         // 是否有備註欄位
    char* _lastUpdate;                  // 最後異動日期
    unsigned int _lastRecord;           // 紀錄最後一筆資料錄的號碼
    unsigned int _dataOffset;           // 儲存資料錄的真正起始位置
    unsigned int _recSize;              // 資料錄的大小
    unsigned int _fieldCount;           // 總共有多少欄位

    // Field
    char** _fieldName;                  // 欄位名稱
    char* _fieldType;                   // 欄位型態
    int* _fieldLen;                     // 欄位長度
    int* _fieldDec;                     // 欄位型態為數值時的小數點位數

    // Records
    char*** _data;                      // 存放資料的地方

  protected:
  public:
    DBaseFile(void) {};
    ~DBaseFile(void) {};
  // member function
    int OpenFile(AnsiString);
    void ReadHeadInfoFromFile(int);
    void ReadFieldInfoFromFile(int);
    void ReadDataFromFile(int);

    bool MemoExist() const { return _memo; }
    char* LastUpdate() const { return _lastUpdate; }
    unsigned int LastRecord() const { return _lastRecord; }
    unsigned int DataOffset() const { return _dataOffset; }
    unsigned int RecordSize() const { return _recSize; }

    unsigned int Fields() const { return _fieldCount; }
    char* FieldName(int idx) const { return _fieldName[idx]; }
    char FieldType(int idx) const { return _fieldType[idx]; }
    int FieldLen(int idx) const { return _fieldLen[idx]; }
    int FieldDec(int idx) const { return _fieldDec[idx]; }

    char* Data(int row, int col) { return _data[row][col]; }
};

#endif
//---------------------------------------------------------------------------
