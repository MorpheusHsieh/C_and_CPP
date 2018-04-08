// Program : 'Decout'
// Author  : Avatar
// Date    : 98.04.13

#define LENGTH 34

void printdec(unsigned long num[]);
int compare(unsigned long lint1[], unsigned long lint2[]);
void division(unsigned long da[], unsigned long db[], unsigned long dq[], unsigned long dr[]);

void decout(unsigned long decp[])
{
   unsigned long *decnum = new unsigned long [21] ;
   unsigned long *tenofnine = new unsigned long [LENGTH+1];
   unsigned long *decq = new unsigned long [LENGTH+1];
   unsigned long *decr = new unsigned long [LENGTH+1];
   unsigned long *temp = new unsigned long [LENGTH+1];
   int decc, deck;
   unsigned long ttn;

   for (decc=0; decc<=20; decc++) {
      decnum[decc] = 0;
      tenofnine[decc] = 0;
   }
   for (decc=0; decc<=LENGTH; decc++, decp++)
      temp[decc] = *decp;

   ttn = 0x3b9aca00;
   tenofnine[1] = ttn;   tenofnine[0] = 1;
   decc = 1;
   while (compare(temp, tenofnine)>0)
   {
      division(temp, tenofnine, decq, decr);
      decnum[0]++;
      decnum[decc] = decr[1];
      decc++;
      for (deck=0; deck<=LENGTH; deck++)
         temp[deck] = decq[deck];
   }
   if ((temp[0])!=0) {
      decnum[decc] = temp[1];
      decnum[0]++;
   }
   printdec(decnum);
}

