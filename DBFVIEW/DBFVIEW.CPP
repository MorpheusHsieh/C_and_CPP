//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("DBFView.res");
USEFORM("Main.cpp", MainForm);
USEUNIT("GetFileName.cpp");
USEUNIT("ShowDBHead.cpp");
USEUNIT("DBF_Head.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
