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
    bool _memo;                         // �O�_���Ƶ����
    char* _lastUpdate;                  // �̫Ყ�ʤ��
    unsigned int _lastRecord;           // �����̫�@����ƿ������X
    unsigned int _dataOffset;           // �x�s��ƿ����u���_�l��m
    unsigned int _recSize;              // ��ƿ����j�p
    unsigned int _fieldCount;           // �`�@���h�����

    // Field
    char** _fieldName;                  // ���W��
    char* _fieldType;                   // ��쫬�A
    int* _fieldLen;                     // ������
    int* _fieldDec;                     // ��쫬�A���ƭȮɪ��p���I���

    // Records
    char*** _data;                      // �s���ƪ��a��

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
