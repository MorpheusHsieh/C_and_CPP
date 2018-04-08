//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "DBaseFile.h"

DBaseFile dbf;
AnsiString FileName = "\0", SQLStr = '\0', TableName = "\0";

void ShowHeaderInfomation(DBaseFile);
void ShowFieldInfomation(DBaseFile);
void ShowRecordData(DBaseFile);
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::OpenClick(TObject *Sender)
{
  bool openfile = false;
  if (Memo2->Modified)
  {
    if (MessageBox(NULL, "�|���s��!�O�_�~��","�T�{����", MB_OKCANCEL) == ID_OK)
     openfile = true;
  }
  else
    openfile = true;

  AnsiString fn, msg;
  if (openfile)
  if (OpenDialog1->Execute())
  {
    fn = OpenDialog1->FileName;
    FileName = OpenDialog1->FileName;
    MainForm->Caption = "DBF���ɵ{�� v1.0 - " + ExtractFileName(FileName);

  }

  int result = dbf.OpenFile(fn);
  if (result == -1)
    msg = "�}�� " + fn + " �ɮץ���...";
  else
  {
    SaveAs->Enabled = true;
    ConvertToText->Enabled = true;
    TabSheet3->Enabled = true;
    msg = "�}�� " + fn + " �ɮצ��\!!!";
  }
  StatusBar1->Panels->Items[0]->Text = msg;

  ShowHeaderInfomation(dbf);
  ShowFieldInfomation(dbf);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveAsClick(TObject *Sender)
{
  if (SaveDialog1->Execute())
  {
    Memo2->Lines->SaveToFile(SaveDialog1->FileName);
    FileName = SaveDialog1->FileName;
    MainForm->Caption = "DBF���ɵ{�� v1.0 - " + ExtractFileName(FileName);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ConvertToTextClick(TObject *Sender)
{
  ShowRecordData(dbf);
}
//---------------------------------------------------------------------------
void ShowHeaderInfomation(DBaseFile dbf)
{
  AnsiString msg;
  // �O�_�� Memo ���
  MainForm->Edit1->Text = (dbf.MemoExist()) ? "��" : "�S��";

  // �̫Ყ�ʤ��
  MainForm->Edit2->Text = dbf.LastUpdate();

  // �̫�@����ƿ������X
  MainForm->Edit3->Text = IntToStr(dbf.LastRecord());

  // ��ƪ��u���_�l��m
  MainForm->Edit4->Text = IntToStr(dbf.DataOffset());

  // �C�@����ƿ�������
  MainForm->Edit5->Text = IntToStr(dbf.RecordSize());
}
//---------------------------------------------------------------------------
void ShowFieldInfomation(DBaseFile dbf)
{
  AnsiString msg;
  MainForm->Memo1->Clear();

  // �`�@���h�ֵ����
  int nField_count = dbf.Fields();
  msg = "�`�@�� " + IntToStr(nField_count) + " �����";
  MainForm->Memo1->Lines->Append(msg);

  // ��ܩҦ�����T
  MainForm->Memo1->Lines->Append("�s�� �W      �� ���A ���� �p���I");
  MainForm->Memo1->Lines->Append("--------------------------------");
  for (int i=0; i<nField_count; i++)
  {
    msg = IntToStr(i+1);
    while(msg.Length() < 5)
      msg += " ";

    String fn(dbf.FieldName(i));
    msg += fn;
    while (msg.Length() < 15)
      msg += " ";

    char ftype = dbf.FieldType(i);
    msg += ftype;
    while (msg.Length() < 21)
      msg += " ";

    msg += IntToStr(dbf.FieldLen(i));
    while (msg.Length() < 26)
      msg += " ";

    msg += IntToStr(dbf.FieldDec(i));
    MainForm->Memo1->Lines->Append(msg);
  }
  MainForm->Memo1->Lines->Append("--------------------------------");
}
//---------------------------------------------------------------------------
void ShowRecordData(DBaseFile dbf)
{
  MainForm->Memo2->Clear();

  AnsiString msg = "";
  int nField_count = dbf.Fields();
  for (int i=0; i<nField_count; i++)
  {
    String field_name(dbf.FieldName(i));
    msg += field_name;
    msg += (i<nField_count-1) ? "," : "";
  }
  MainForm->Memo2->Lines->Append(msg);

  int recs = dbf.LastRecord();
  int fields = dbf.Fields();
  for (int row=0; row<recs; row++)
  {
    char* sign = dbf.Data(row,0);
    if ((char)sign[0] != '*')
    {
      msg = "";
      for (int j=0; j<fields; j++)
      {
        int col = j+1; // �]���h�F�@�ӧR�����
        String data(dbf.Data(row, col));
        if (dbf.FieldType(j) == 'C')
          msg += "\"" + data + "\"";
        else
          msg += data;
        msg += (col != fields) ? "," : "";
      }
      MainForm->Memo2->Lines->Append(msg);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Button1Click(TObject *Sender)
{
  if (Edit6->Text.Length() == 0)
  {
    if (MessageBox(NULL, "���W�٬��ť�, �п�J...", "�T�{����", MB_OK) == ID_OK)
      Edit6->SetFocus();
  }
  else
  {
    TableName = Edit6->Text;
    AnsiString sqlstr = "CREATE TABLE " + TableName + " (";
    int fields = dbf.Fields();
    for (int i=0; i<fields; i++)
    {
      char type = dbf.FieldType(i);
      int len = dbf.FieldLen(i);
      int dec = dbf.FieldDec(i);
      sqlstr += dbf.FieldName(i);
      switch(type)
      {
        case 'C': sqlstr += " VARCHAR(" + IntToStr(len) + ")";
                  break;
        case 'N': if (dec == 0)
                    sqlstr += " INT";
                  else if (dec != 0)
                    sqlstr += " DOUBLE";
                  break;
        case 'B': sqlstr += " BINARY";
                  break;
        case 'L': sqlstr += " BIT";
                  break;
        case 'D': sqlstr += " DATETIME";
                  break;
      }
      if (i < fields-1) sqlstr += ", ";
    }
    sqlstr += ")";
    SQLStr = sqlstr;
    MainForm->Memo3->Clear();
    MainForm->Memo3->Lines->Append(sqlstr);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormActivate(TObject *Sender)
{
  PageControl1->ActivePage = TabSheet1;
  TabSheet3->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Button2Click(TObject *Sender)
{
  Query1->Close();
  Query1->SQL->Clear();
  Query1->SQL->Add(SQLStr);
  Query1->Open();
}
//---------------------------------------------------------------------------

