//---------------------------------------------------------------------------
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "Main.h"
#include "DBaseFile.h"
//---------------------------------------------------------------------------
int DBaseFile::OpenFile(AnsiString fn)
{
  int handle = -1;
  if ((handle = open(fn.c_str(), O_BINARY|O_RDONLY)) != -1)
  {
    ReadHeadInfoFromFile(handle);
    ReadFieldInfoFromFile(handle);
    ReadDataFromFile(handle);
  }
  close(handle);
  return handle;
}
//---------------------------------------------------------------------------
void DBaseFile::ReadHeadInfoFromFile(int handle)
{
  DBF_HEAD head;

  lseek(handle, 0L, SEEK_SET);                // 將指標移向檔案起始位置
  read(handle, (char*)&head, sizeof(head));   // 讀入DBF檔頭資訊

  // 是否有備註欄位
  _memo = (head.dbf_id == (char)0x83) ? true : false;

  // 最後一次異動日期
  int yy, mm, dd;
  yy = head.last_update[0] + 1900;
  mm = head.last_update[1];
  dd = head.last_update[2];
  String sDate(IntToStr(yy)+'/'+IntToStr(mm)+'/'+IntToStr(dd));
  int size = sDate.Length();
  _lastUpdate = new char [size];
  strcpy(_lastUpdate, sDate.c_str());

  // 最後一筆資料錄的號碼
  _lastRecord = head.last_rec;

  // 資料儲存的真正起始位置
  _dataOffset = head.data_offset;

  // 資料錄的大小
  _recSize = head.rec_size;
}
//---------------------------------------------------------------------------
void DBaseFile::ReadFieldInfoFromFile(int handle)
{
  FIELD_REC field_rec;

  // 總共有多少筆欄位
  _fieldCount = (_dataOffset - _HeaderSize)/_SizePerField;
  _fieldName = new char* [_fieldCount];
  _fieldType = new char[_fieldCount];
  _fieldLen = new int[_fieldCount];
  _fieldDec = new int[_fieldCount];

  // 開始取出欄位資訊
  int cnt = 0;
  bool more_fields = true;
  lseek(handle, (long)_HeaderSize, SEEK_SET); // 將指標移向檔案起始位置
  while (more_fields)
  {
    more_fields = (read(handle, (char*)&field_rec,
                   sizeof(FIELD_REC)) == sizeof(FIELD_REC));
    if (more_fields)
    {
      more_fields = (field_rec.field_name[0] != 0x0D);
      if (more_fields)
      {
        int len = strlen(field_rec.field_name);
        _fieldName[cnt] = new char[len+1];
        strcpy(_fieldName[cnt], field_rec.field_name);

        _fieldType[cnt] = field_rec.field_type;
        switch(_fieldType[cnt])
        {
          case 'N':
            _fieldLen[cnt] = field_rec.len_info.num_size.len;
            _fieldDec[cnt] = field_rec.len_info.num_size.dec;
            break;
          default:
            _fieldLen[cnt] = field_rec.len_info.char_len;
            _fieldDec[cnt] = 0;
        }
        cnt++;
      }
    }
  }
}
//---------------------------------------------------------------------------
void DBaseFile::ReadDataFromFile(int handle)
{
  // 配置存放資料的記憶體空間
  int recs = _lastRecord;
  _data = new char** [recs];

  int rec_size = _recSize;
  char* record = new char [rec_size];
  long data_offset = _dataOffset;
  lseek(handle, data_offset, SEEK_SET);

  AnsiString msg;
  int rec_offset, field_len, fields = _fieldCount, array_size;
  for(int row=0; row<recs; row++)
  {
    array_size = fields+1;              // 多了一個刪除欄位
    _data[row] = new char* [array_size];
    read(handle, record, rec_size);     // 從檔案中讀取資料

    // 確認該筆資料是否為有效欄位，0x20->有效，'*'->已刪除
    _data[row][0] = new char [2];
    strncpy(_data[row][0], record, 1);
    _data[row][0][1] = '\0';

    rec_offset = 1;
    for (int j=0; j<fields; j++)
    {
      char type = _fieldType[j];
      switch(type)
      {
        case 'C':
        case 'N': field_len = _fieldLen[j];
                  array_size = field_len + 1;
                  break;
        case 'B': field_len = 10;
                  array_size = field_len + 1;
                  break;
        case 'L': field_len = 1;
                  array_size = field_len + 1;
                  break;
        case 'D': field_len = 8;
                  array_size = field_len + 3;
                  break;
      }
      int col = j + 1; // 因為多了一個刪除記號欄位，所以data欄位要向右移一位
      _data[row][col] = new char [array_size];
      strncpy(_data[row][col], (char*)&record[rec_offset], field_len);
      _data[row][col][array_size-1] = '\0';     // 字串尾巴補'\0'(結束符號)

      if (type == 'D') // 處理欄位型態為日期時0
      {
        MainForm->Memo2->Lines->Append(_data[row][col]);
        char* yy = new char [5]; strncpy(yy, &_data[row][col][0],4);
        char* mm = new char [3]; strncpy(mm, &_data[row][col][4],2);
        char* dd = new char [3]; strncpy(dd, &_data[row][col][6],2);
        char* date = new char [11];
        strcpy(date, yy); date[4] = '/'; date[5] = '\0';
        strcat(date, mm); date[7] = '/'; date[8] = '\0';
        strcat(date, dd); date[array_size-1] = '\0';
        strcpy(_data[row][col], date);
      }
      else if (type == 'C') // 當字串中有雙引號時（"）, 清除掉
      {
        char ch = '\0';
        char* buf = new char[array_size];
        int out_posi = 0, in_posi = 0;
        bool result = true;
        do {
          ch = _data[row][col][out_posi++];
          if (ch != 0x22) // '"' 字元
            buf[in_posi++] = ch;
          if (!(out_posi < array_size))
            result = false;
        } while(result);
        buf[in_posi] = '\0';
        strcpy(_data[row][col], buf);
      }
      rec_offset += field_len;
    }
  }
}
//---------------------------------------------------------------------------

