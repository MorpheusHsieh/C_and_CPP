//Program : Simulation of an air defense naval engagement
//Auther  : Avatar
//Date    : 97/11/17
//Filename: SeaFox1.cpp

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WarTime 1000

int Hit, Through;
int AllHit, AllThrough;
// Initially Surveillence Radar
int Tmax    = 10200;   // M
int Tm      = 15;      // Sec
int Tv      = 3;       // Sec
// Initially Single-Channel Track Radar
int m       = 5;
int c       = 3;
int SFmax   = 8000;    // M
int SFmin   = 1500;    // M
int SFs     = 600;     // M/Sec
int SFrt    = 2;       // Sec
int SFrpt   = 2;       // Sec
const SFN   = 2;       // SeaFox Number
float SFrp  = 0.8;
float SFhp  = 0.9;
// Initially Missile
int SXs     = 300;     // M/Sec
int SXi     = 6;       // Sec
const SXN   = 6;       // Missile number
// Initially Frigate
float FRhp  = 0.4;
int SFn = SFN;
int SXn = SXN;

int SeaFox[SFN+1];
struct Missile {
       int No;
       int Life;
       int State;
       int NextState;
       float ClockTime;
       } Ax[SXN+1];

void Detect(void);
void Acquaint(void);
void Fire(void);
void Contact(void);
void Killer(void);
void Miss(void);
void Repeat(void);
void Frigate(void);
void ClockSort(void);
void InitialState(void);
void DisplayState(void);
float Random(void);

void main(void)
{
   clrscr();

   randomize();
   printf("Simulation of an air defense naval engagement.");

   FILE *outfile;
   char filename[81];
   strcpy(filename, "c:\\temp\\SeaFox7.txt");

   // Open for the file for reading
   if ( (outfile = fopen(filename, "w")) == NULL )
   {
      printf("fopen failed.\n");
      exit(0);
   }

   for (SFmin=1000; SFmin<=1700; SFmin+=100)
   {
      printf("\nSFmin:%4d,", SFmin);
      fprintf(outfile, "\nSFmin:%4d,", SFmin);
      for (int i=0; i<=SXn; i++)
      {
	 printf("%5d,", i);
	 fprintf(outfile, "%5d,", i);
	 if ( i == 9 )
	 {
	    printf("\n        ");
	    fprintf(outfile, "\n        ");
	 }
      }
      printf("  All  A/1000 A/6000");
      fprintf(outfile, "  All  A/1000 A/6000");
      printf("\n-------------------------------------------------------------------------------");
      fprintf(outfile, "\n-------------------------------------------------------------------------------");

      // Initial Hit & Through number
      int hit[SXN+1], th[SXN+1];
      for (i=0; i<=SXn; i++)
      {
	 hit[i] = 0;
	 th[i] = 0;
      }
      AllHit = 0;
      AllThrough = 0;
      for (i=0; i<WarTime; i++)
      {
	 InitialState();
	 int AxKillNum = 0;
	 Hit = 0; Through = 0;
	 while ( AxKillNum < SXn )
	 {
//          DisplayState();
//          getch();
	    if ( Ax[0].Life == 0 )
	    {
	       switch ( Ax[0].NextState )
	       {
		  case 1:
		     Detect();
		     break;
		  case 2:
		     Acquaint();
		     break;
		  case 3:
		     Fire();
		     break;
		  case 4:
		     Contact();
		     break;
		  case 5:
		     Killer();
		     AxKillNum ++;
		     break;
		  case 6:
		     Miss();
		     break;
		  case 7:
		     Repeat();
		     break;
		  case 8:
		     Frigate();
		     Through ++;
		     break;
		  case 9:
		     Ax[0].State = 9;
		     break;
		  case 10:
		     Ax[0].State = 10;
	       }
	    }
	    else if ( Ax[0].Life == 1 )
	       AxKillNum++;
	    ClockSort();
	 }
	 hit[Hit]++;
	 th[Through]++;
	 AllHit += Hit;
	 AllThrough += Through;
      }
      fprintf(outfile, "\nHit      : ");
      printf("\nHit      : ");
      for (i=0; i<=SXn; i++)
      {
	 fprintf(outfile, "%5d,", hit[i]);
	 printf("%5d,", hit[i]);
	 if ( i == 9 )
	 {
	    fprintf(outfile, "\n           ");
	    printf("\n           ");
	 }
      }
      fprintf(outfile, "%5d,", AllHit);
      fprintf(outfile, "  %2.3f", AllHit/1000.0);
      fprintf(outfile, "  %2.3f", AllHit/6000.0);
      printf("%5d", AllHit);
      printf("  %2.3f", AllHit/1000.0);
      printf("  %2.3f", AllHit/6000.0);

      fprintf(outfile, "\nThough   : ");
      printf("\nThough   : ");
      for (i=0; i<=SXn; i++)
      {
	 fprintf(outfile, "%5d,", th[i]);
	 printf("%5d,", th[i]);
	 if ( i == 9 )
	 {
	    fprintf(outfile, "\n           ");
	    printf("\n           ");
	 }
      }
      fprintf(outfile, "%5d,", AllThrough);
      fprintf(outfile, "  %2.3f", AllThrough/1000.0);
      fprintf(outfile, "  %2.3f\n", AllThrough/6000.0);
      printf("%5d", AllThrough);
      printf("  %2.3f", AllThrough/1000.0);
      printf("  %2.3f\n", AllThrough/6000.0);
//      getch();
   }
   fclose(outfile);
   printf("\nGame Over...");
// getch();
}

void InitialState(void)
{
   for (int i=0; i<SFn; i++)
      SeaFox[i] = 9999;
   for (i=0; i<SXn; i++)
   {
      Ax[i].No = i;
      Ax[i].Life = 0;
      Ax[i].State = 0;
      Ax[i].NextState = 1;
      Ax[i].ClockTime = i * SXi;
   }
}

void DisplayState(void)
{
   char *ch;
   for (int i=0; i<SXn; i++)
   {
      printf("\nAx[%d]: ", Ax[i].No);
      switch (Ax[i].State)
      {
         case 0:
            ch = "Start  ";
	    break;
	 case 1:
            ch = "Detect ";
            break;
         case 2:
            ch = "Lock   ";
            break;
         case 3:
            ch = "Fire   ";
            break;
         case 4:
            ch = "INT    ";
            break;
         case 5:
            ch = "Killer ";
            break;
         case 6:
            ch = "Miss   ";
            break;
         case 7:
	    ch = "Repeat ";
	    break;
         case 8:
            ch = "Through";
            break;
         case 9:
            ch = "Hit    ";
            break;
         case 10:
            ch = "Not Hit";
            break;
         case 11:
            ch = "Idle   ";
            break;
         case 12:
            ch = "Dead   ";
            break;
         default:
            ch = "O.C    ";
      }
      printf("Clock Time:%2.4f,\t", Ax[i].ClockTime);
      printf("State:%s, ", ch);
      printf("Life: ");
      if ( Ax[i].Life == 0 )
         printf("Alive");
      else if (Ax[i].Life == 1)
         printf("Dead ");
   }
   printf("\n");
}

float Random(void)
{
   return ((random(20000)/10000.0) - 1.0);
}

void ClockSort(void)
{
   struct Missile AxTemp;
   for (int i=0; i<SXn-1; i++)
   {
      for (int j=i+1; j<SXn; j++)
      {
         if ( Ax[i].ClockTime > Ax[j].ClockTime )
         {
            AxTemp.No = Ax[i].No;
            AxTemp.Life = Ax[i].Life;
            AxTemp.State = Ax[i].State;
            AxTemp.NextState = Ax[i].NextState;
            AxTemp.ClockTime = Ax[i].ClockTime;
            Ax[i].No = Ax[j].No;
            Ax[i].Life = Ax[j].Life;
            Ax[i].State = Ax[j].State;
            Ax[i].NextState = Ax[j].NextState;
            Ax[i].ClockTime = Ax[j].ClockTime;
            Ax[j].No = AxTemp.No;
            Ax[j].Life = AxTemp.Life;
            Ax[j].State = AxTemp.State;
            Ax[j].NextState = AxTemp.NextState;
            Ax[j].ClockTime = AxTemp.ClockTime;
	 }
      }
   }
}

void Detect(void)
{
   float DetecTime = Tm + Random() * Tv;
   Ax[0].ClockTime += DetecTime;
   Ax[0].State = 1;
   Ax[0].NextState = 2;
//   printf("Detect Time:%f, ", DetecTime);
}

void Acquaint(void)
{
   float AcqTime = m + c * Random();
   Ax[0].ClockTime += AcqTime;
   Ax[0].State = 2;
   Ax[0].NextState = 3;
//   printf("Acqaint Time:%f, ", AcqTime);
}

void Fire(void)
{
   int SeaFoxBusyNum = 0;
   int SeaFoxAttackExist;
   for (int i=0; i<SFn; i++) {
      if ( SeaFox[i] != 9999 ) {
         if ( SeaFox[i] == Ax[0].No )
            SeaFoxAttackExist = 1;
         else
            SeaFoxBusyNum ++;
      }
   }

   if ( SeaFoxAttackExist != 1 )
   {
      if ( SeaFoxBusyNum < SFn )
      {
	 float FlyTime = Ax[0].ClockTime - Ax[0].No * SXi;
	 float dist = Tmax - FlyTime * SXs;
         if ( dist > SFmax )
         {
            float WaitTime = (dist - SFmax) / SXs;
            Ax[0].ClockTime += WaitTime;
            Ax[0].State = 3;
            Ax[0].NextState = 4;
         }
         else if ( (dist <= SFmax) && (dist >= SFmin) )
         {
            int count = 0;
            while ( (SeaFox[count] != 9999) && (count < SFn) )
               count ++;
            SeaFox[count] = Ax[0].No;
            Ax[0].State = 3;
            Ax[0].NextState = 4;
         }
         else if ( dist < SFmin )
         {
	    Ax[0].State = 8;
	    Ax[0].NextState = 8;
         }
      }
      else if ( SeaFoxBusyNum >= SFn )
      {
         for (i=0; i<SFn; i++)
         {
            if ( SeaFox[i] != Ax[0].No )
            {
               Ax[0].State = 11;
               Ax[0].ClockTime = 9998;
            }
         }
      }
   }
   else
   {
      Ax[0].State = 4;
      Ax[0].NextState = 4;
   }

//   for ( i=0; i<SFn; i++)
//      printf("SeaFox[%d]: %d, ", i, SeaFox[i]);
}

void Contact(void)
{
   float FlyTime = Ax[0].ClockTime - Ax[0].No * SXi;
   float dist = Tmax - FlyTime * SXs;
   float EventTime = dist / ( SFs + SXs );
   Ax[0].ClockTime += EventTime;

   float IntProb = random(10000) /10000.0;
   if ( IntProb < ( SFrp * SFhp ) )
   {
      Ax[0].State = 5;
      Ax[0].NextState = 5;
   }
   else if ( IntProb >= ( SFrp * SFhp ) )
   {
      Ax[0].State = 6;
      Ax[0].NextState = 6;
   }
//   printf("Contact Time:%f, Prob:%f", EventTime, IntProb);
}

void Killer(void)
{
   int SystemTime;
   for (int i=0; i<SFn; i++)
      if ( SeaFox[i] == Ax[0].No )
         SeaFox[i] = 9999;

   Ax[0].Life = 1;
   Ax[0].State = 12;
   SystemTime = Ax[0].ClockTime;
   Ax[0].ClockTime = 9999;

   int sfno = i;
   int count = 0;
   while ( (Ax[count].NextState != 8) && (count<SFn) )
      count ++;

   if ( count < SFn )
   {
      Ax[count].State = 3;
      Ax[count].NextState = 3;
      Ax[count].ClockTime = SystemTime;
      SeaFox[sfno] = Ax[count].No;
   }

//   for ( i=0; i<SFn; i++)
//      printf("SeaFox[%d]: %d, ", i, SeaFox[i]);
}

void Miss(void)
{
   Ax[0].ClockTime += SFrt;
   float dist = Tmax - (Ax[0].ClockTime - Ax[0].No * SXi) * SXs;
   if ( dist >= SFmin )
   {
      Ax[0].State = 7;
      Ax[0].NextState = 7;
   }
   else if ( dist < SFmin )
   {
      Ax[0].State = 8;
      Ax[0].NextState = 8;
      for (int i=0; i<SFn; i++)
      {
         if ( SeaFox[i] == Ax[0].No )
            SeaFox[i] = 9999;
      }
   }
//   printf("Dist: %f, ", dist);
}

void Repeat(void)
{
   Ax[0].ClockTime += SFrpt;
   float dist = Tmax - (Ax[0].ClockTime - Ax[0].No * SXi) * SXs;
   if ( dist >= SFmin )
   {
      Ax[0].State = 3;
      Ax[0].NextState = 3;
   }
   else if ( dist < SFmin )
   {
      Ax[0].State = 8;
      Ax[0].NextState = 8;
   }
//   printf("Dist: %f, ", dist);
}

void Frigate(void)
{
   for (int i=0; i<SFn; i++)
      if ( SeaFox[i] == Ax[0].No )
	 SeaFox[i] = 9999;

   float HitFrigateProb = random(10000) / 10000.0;
   if ( HitFrigateProb <= FRhp )
   {
      Ax[0].State = 9;
      Hit += 1;
   }
   else
      Ax[0].State = 10;
   Ax[0].Life = 1;
   Ax[0].ClockTime = 9999;

//   printf("Prob: %f, ", HitFrigateProb);
//   for ( i=0; i<SFn; i++)
//      printf("SeaFox[%d]: %d, ", i, SeaFox[i]);
}
