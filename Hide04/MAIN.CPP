//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "MAIN.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------

int scale;
int d;

struct{
    float x,y,z;
}   A,B,C,D;

struct{
    float l,m,n;
}   Vector1,Vector2,NORMAL,View_axis;

struct{
    int x,y,z;
}   P1,P2,P3,P4,COP;

struct{
    float x,y,z;
}   PP1,PP2,PP3,PP4;

struct{
    float A[4],B[4],C[4],D[4];
} VIEW;

__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    Form1->Height = 768;
    Form1->Width  = 1024;
    Form1->Top = 0;
    Form1->Left = 0;
    PageControl1->ActivePage = TabSheet1;
    Table1->Active = true;
    Table2->Active = true;
    View_axis.l = 0.0;
    View_axis.m = 0.0;
    View_axis.n = 30.0;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button1Click(TObject *Sender)
{
    Table3->Active = false;
    Table3->EmptyTable();
    Table3->Active = true;
    int mesh[4][4],z,f;
    scale= 40;
    POINT point1[4],point2[4],point3[4];
    PaintBox1->Canvas->Brush->Color = clSilver;
    PaintBox2->Canvas->Brush->Color = clSilver;
    PaintBox3->Canvas->Brush->Color = clSilver;
    z = 1;
    f = 1;

    Table2->First();

    do{
        for(int i = 0;i < 4;i++){
            for(int j = 0;j < 4;j++){
                mesh[i][j]=Table2->Fields[z]->AsInteger;
                z++;
            }
        }

        for(int i = 0;i < 3;i++){
            for(int j = 0;j < 3;j++){
                Table3->Append();
                Table3->FieldValues["A"] = mesh[i][j];
                Table3->FieldValues["B"] = mesh[i][j+1];
                Table3->FieldValues["C"] = mesh[i+1][j+1];
                Table3->FieldValues["D"] = mesh[i+1][j];
                Table3->Post();
            }
        }
        z = 1;
        Table2->Next();
        f++;
    }   while(f <= 32);

    Table3->First();
    ProgressBar1->Max = Table3->RecordCount;
    ProgressBar1->Position = 0;

    for(int i = 1;i <= ProgressBar1->Max;i++){
        ProgressBar1->Position++;

        Table1->RecNo = Table3->FieldValues["A"];
        P1.x = Table1->FieldValues["X"]*scale;
        P1.y = Table1->FieldValues["Y"]*scale;
        P1.z = Table1->FieldValues["Z"]*scale;

        Table1->RecNo = Table3->FieldValues["B"];
        P2.x = Table1->FieldValues["X"]*scale;
        P2.y = Table1->FieldValues["Y"]*scale;
        P2.z = Table1->FieldValues["Z"]*scale;

        Table1->RecNo = Table3->FieldValues["C"];
        P3.x = Table1->FieldValues["X"]*scale;
        P3.y = Table1->FieldValues["Y"]*scale;
        P3.z = Table1->FieldValues["Z"]*scale;

        Table1->RecNo = Table3->FieldValues["D"];
        P4.x = Table1->FieldValues["X"]*scale;
        P4.y = Table1->FieldValues["Y"]*scale;
        P4.z = Table1->FieldValues["Z"]*scale;
/*
        Vector1.l = P2.x-P1.x;
        Vector1.m = P2.y-P1.y;
        Vector1.n = P2.z-P1.z;
        Vector2.l = P2.x-P3.x;
        Vector2.m = P2.y-P3.y;
        Vector2.n = P2.z-P3.z;
        NORMAL.l = Vector1.m*Vector2.n-Vector2.m*Vector1.n;
        NORMAL.m = Vector1.n*Vector2.l-Vector2.n*Vector1.l;
        NORMAL.n = Vector1.l*Vector2.m-Vector2.l*Vector1.m;
        Table3->Edit();
        Table3->FieldValues["L"] = NORMAL.l;
        Table3->FieldValues["M"] = NORMAL.m;
        Table3->FieldValues["N"] = NORMAL.n;
        Table3->Post();
*/
        COP.x = 158;
        COP.y = 143;
        point1[0] = Point(P1.x+COP.x,P1.y+COP.y);
        point1[1] = Point(P2.x+COP.x,P2.y+COP.y);
        point1[2] = Point(P3.x+COP.x,P3.y+COP.y);
        point1[3] = Point(P4.x+COP.x,P4.y+COP.y);
        PaintBox1->Canvas->Polygon(point1, 3);

        COP.y = 200;
        point2[0] = Point(P1.x+COP.x,-P1.z+COP.y);
        point2[1] = Point(P2.x+COP.x,-P2.z+COP.y);
        point2[2] = Point(P3.x+COP.x,-P3.z+COP.y);
        point2[3] = Point(P4.x+COP.x,-P4.z+COP.y);
        PaintBox2->Canvas->Polygon(point2, 3);

        point3[0] = Point(P1.y+COP.x,-P1.z+COP.y);
        point3[1] = Point(P2.y+COP.x,-P2.z+COP.y);
        point3[2] = Point(P3.y+COP.x,-P3.z+COP.y);
        point3[3] = Point(P4.y+COP.x,-P4.z+COP.y);
        PaintBox3->Canvas->Polygon(point3, 3);

        Table3->Next();
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button2Click(TObject *Sender)
{
    PaintBox4->Refresh();
    Table1->Active = true;
    Table3->Active = true;
    COP.x = 300;
    COP.y = 300;
    POINT point[4];
    float th_z,th_y,th_x,a,View_Div;
    a = 3.14/180;
    th_x = StrToFloat(Edit1->Text)*a;
    th_y = StrToFloat(Edit2->Text)*a;
    th_z = StrToFloat(Edit3->Text)*a;
    VIEW.A[0] = cos(th_z)*cos(th_y);
    VIEW.A[1] = sin(th_z)*cos(th_x)+cos(th_z)*sin(th_y)*sin(th_x);
    VIEW.A[2] = sin(th_z)*cos(th_x)-cos(th_z)*sin(th_y)*cos(th_x);
    VIEW.A[3] = 0;
    VIEW.B[0] = -sin(th_z)*cos(th_y);
    VIEW.B[1] = cos(th_z)*cos(th_x)-sin(th_z)*sin(th_y)*sin(th_x);
    VIEW.B[2] = cos(th_z)*sin(th_x)+sin(th_z)*sin(th_y)*cos(th_x);
    VIEW.B[3] = 0;
    VIEW.C[0] = sin(th_y);
    VIEW.C[1] = -cos(th_y)*sin(th_x);
    VIEW.C[2] = cos(th_y)*cos(th_x);
    VIEW.C[3] = 0;
    VIEW.D[0] = 0;
    VIEW.D[1] = 0;
    VIEW.D[2] = 0;
    VIEW.D[3] = 1;

    Table3->Last();
    ProgressBar2->Max = Table3->RecordCount;
    ProgressBar2->Position = 0;
    for(int i = 1;i <= ProgressBar1->Max;i++){
        ProgressBar2->Position++;

        Table1->RecNo = Table3->FieldValues["A"];
        P1.x = Table1->FieldValues["X"]*scale;
        P1.y = Table1->FieldValues["Y"]*scale;
        P1.z = Table1->FieldValues["Z"]*scale;
        PP1.x = P1.x*VIEW.A[0]+P1.y*VIEW.A[1]+P1.z*VIEW.A[2]+VIEW.A[3];
        PP1.y = P1.x*VIEW.B[0]+P1.y*VIEW.B[1]+P1.z*VIEW.B[2]+VIEW.B[3];
        PP1.z = P1.x*VIEW.C[0]+P1.y*VIEW.C[1]+P1.z*VIEW.C[2]+VIEW.C[3];

        Table1->RecNo = Table3->FieldValues["B"];
        P2.x = Table1->FieldValues["X"]*scale;
        P2.y = Table1->FieldValues["Y"]*scale;
        P2.z = Table1->FieldValues["Z"]*scale;
        PP2.x = P2.x*VIEW.A[0]+P2.y*VIEW.A[1]+P2.z*VIEW.A[2]+VIEW.A[3];
        PP2.y = P2.x*VIEW.B[0]+P2.y*VIEW.B[1]+P2.z*VIEW.B[2]+VIEW.B[3];
        PP2.z = P2.x*VIEW.C[0]+P2.y*VIEW.C[1]+P2.z*VIEW.C[2]+VIEW.C[3];

        Table1->RecNo = Table3->FieldValues["C"];
        P3.x = Table1->FieldValues["X"]*scale;
        P3.y = Table1->FieldValues["Y"]*scale;
        P3.z = Table1->FieldValues["Z"]*scale;
        PP3.x = P3.x*VIEW.A[0]+P3.y*VIEW.A[1]+P3.z*VIEW.A[2]+VIEW.A[3];
        PP3.y = P3.x*VIEW.B[0]+P3.y*VIEW.B[1]+P3.z*VIEW.B[2]+VIEW.B[3];
        PP3.z = P3.x*VIEW.C[0]+P3.y*VIEW.C[1]+P3.z*VIEW.C[2]+VIEW.C[3];

        Table1->RecNo = Table3->FieldValues["D"];
        P4.x = Table1->FieldValues["X"]*scale;
        P4.y = Table1->FieldValues["Y"]*scale;
        P4.z = Table1->FieldValues["Z"]*scale;
        PP4.x = P4.x*VIEW.A[0]+P4.y*VIEW.A[1]+P4.z*VIEW.A[2]+VIEW.A[3];
        PP4.y = P4.x*VIEW.B[0]+P4.y*VIEW.B[1]+P4.z*VIEW.B[2]+VIEW.B[3];
        PP4.z = P4.x*VIEW.C[0]+P4.y*VIEW.C[1]+P4.z*VIEW.C[2]+VIEW.C[3];
/*
        Vector1.l = PP2.x-PP1.x;
        Vector1.m = PP2.y-PP1.y;
        Vector1.n = PP2.z-PP1.z;
        Vector2.l = PP2.x-PP3.x;
        Vector2.m = PP2.y-PP3.y;
        Vector2.n = PP2.z-PP3.z;
        NORMAL.l = Vector1.m*Vector2.n-Vector2.m*Vector1.n;
        NORMAL.m = Vector1.n*Vector2.l-Vector2.n*Vector1.l;
        NORMAL.n = Vector1.l*Vector2.m-Vector2.l*Vector1.m;
        View_Div = (View_axis.l*View_axis.l+View_axis.m*View_axis.m+View_axis.n*View_axis.n)/(sqrt(pow(View_axis.l,2)+pow(View_axis.m,2)+pow(View_axis.n,2))*sqrt(pow(View_axis.l,2)+pow(View_axis.m,2)+pow(View_axis.n,2)));
        if(View_Div >= 0 && View_Div <= 1){
            point[0] = Point((int)2*PP1.x+COP.x,(int)2*PP1.y+COP.y);
            point[1] = Point((int)2*PP2.x+COP.x,(int)2*PP2.y+COP.y);
            point[2] = Point((int)2*PP3.x+COP.x,(int)2*PP3.y+COP.y);
            point[3] = Point((int)2*PP4.x+COP.x,(int)2*PP4.y+COP.y);
            PaintBox4->Canvas->Polygon(point, 3);
        }*/
        PaintBox4->Canvas->MoveTo((int)2*PP1.x+COP.x,(int)2*PP1.y+COP.y);
        PaintBox4->Canvas->LineTo((int)2*PP2.x+COP.x,(int)2*PP2.y+COP.y);
        PaintBox4->Canvas->LineTo((int)2*PP3.x+COP.x,(int)2*PP3.y+COP.y);
        PaintBox4->Canvas->LineTo((int)2*PP4.x+COP.x,(int)2*PP4.y+COP.y);
        PaintBox4->Canvas->LineTo((int)2*PP1.x+COP.x,(int)2*PP1.y+COP.y);

        Table3->Prior();
    }
}
//---------------------------------------------------------------------------

