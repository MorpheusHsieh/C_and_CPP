//---------------------------------------------------------------------------

#include <io.h>
#include <vcl.h>
#include <fcntl.h>
#pragma hdrstop

#include "ShowDBHead.h"
#include "Main.h"
#include "DBF_Head.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

void ShowDBHead(DBF_HEAD dbf_head, AnsiString fn)
{
    AnsiString msg;
    msg = "資料庫檔頭長度為" + IntToStr(sizeof(dbf_head));
    MainForm->Memo1->Lines->Append(msg);

    int handle;
    if ((handle = open(fn.c_str(),O_RDONLY | O_BINARY)) == -1)
    {
      MainForm->StatusBar->Panels->Items[0]->Text = "檔案開啟失敗...";
      MessageBeep(0);
      exit(1);
    }

    if (read(handle, (char*)&dbf_head, sizeof(dbf_head)) != sizeof(DBF_HEAD))
    {
      MainForm->StatusBar->Panels->Items[0]->Text = "檔案讀取錯誤...";
      MessageBeep(0);
      exit(1);
    }

    MainForm->Memo1->Lines->Append("");
    msg = "檔案名稱：" + fn;
    MainForm->Memo1->Lines->Append(msg);

    //檢驗是否有備註欄位
    char memo = 0x03, no_memo = 0x83;
    msg = "備註：";
    if (dbf_head.dbf_id == (char)memo)
      msg += "沒有";
    else if (dbf_head.dbf_id == (char)no_memo)
      msg += "有";
    MainForm->Memo1->Lines->Append(msg);

    //檢驗資料庫最後異動日期
    int yy, mm, dd;
    yy = dbf_head.last_update[0];
    mm = dbf_head.last_update[1];
    dd = dbf_head.last_update[2];
    msg = "最後異動日期：";
    msg = msg + IntToStr(1900+yy) + "/";
    if (mm<=9) msg += "0";
    msg = msg + IntToStr(mm) + "/";
    if (dd<=9) msg += "0";
    msg = msg + IntToStr(dd);
    MainForm->Memo1->Lines->Append(msg);

    //顯示最後一筆資料錄的號碼
    long record_cnt = dbf_head.last_rec;
    msg = "此資料檔共有 " + IntToStr(record_cnt) + " 筆資料";
    MainForm->Memo1->Lines->Append(msg);

    //顯示儲存資料錄真正的起始位址
    msg = "儲存資料的起始位址為 " + IntToStr(dbf_head.data_offset);
    MainForm->Memo1->Lines->Append(msg);

    //顯示每筆資料的長度
    msg = "每筆資料的長度 " + IntToStr(dbf_head.rec_size) + " 位元";
    MainForm->Memo1->Lines->Append(msg);
}
//---------------------------------------------------------------------------
