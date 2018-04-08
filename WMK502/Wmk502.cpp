//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("Wmk502.res");
USEFORM("Message.cpp", MesgForm);
USEFORM("MDIChild.cpp", MDIForm);
USEUNIT("GetFileName.cpp");
USEUNIT("FuncToggle.cpp");
USEUNIT("Raw2BMP.cpp");
USEUNIT("Sobel.cpp");
USEUNIT("Matrix.cpp");
USEFORM("Morphism.cpp", MorphismForm);
USEFORM("CycleTms.cpp", CycleTmsForm);
USEFORM("PriorityBk.cpp", PriorityForm);
USEFORM("WmkImg.cpp", WmkImgForm);
USEFORM("SliceHelp.cpp", SliceHelpForm);
USEUNIT("WmkHide502.cpp");
USEUNIT("ZigZag.cpp");
USEUNIT("Fast_DCT.cpp");
USEUNIT("Cosft2.cpp");
USEUNIT("Vector.cpp");
USEUNIT("realft.cpp");
USEFORM("Nrerror.cpp", ErrorMsgForm);
USEUNIT("four1.cpp");
USEFORM("Moment.cpp", MomentForm);
USEFORM("Main.cpp", MainForm);
USEFORM("ImgStrtPosi.cpp", ImgPosiForm);
USEUNIT("WmkDeHde502.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        Application->Initialize();
        Application->CreateForm(__classid(TMainForm), &MainForm);
        Application->CreateForm(__classid(TMesgForm), &MesgForm);
        Application->CreateForm(__classid(TMorphismForm), &MorphismForm);
        Application->CreateForm(__classid(TCycleTmsForm), &CycleTmsForm);
        Application->CreateForm(__classid(TPriorityForm), &PriorityForm);
        Application->CreateForm(__classid(TWmkImgForm), &WmkImgForm);
        Application->CreateForm(__classid(TSliceHelpForm), &SliceHelpForm);
        Application->CreateForm(__classid(TErrorMsgForm), &ErrorMsgForm);
        Application->CreateForm(__classid(TMomentForm), &MomentForm);
        Application->CreateForm(__classid(TImgPosiForm), &ImgPosiForm);
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
