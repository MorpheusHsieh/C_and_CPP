// Program : 'Compare' is to compare two integer
// Author  : Avatar
// Date    : 98.04.07

int compare(unsigned long lint1[], unsigned long lint2[])
{
   int flag = 0;
   unsigned long length;

   if ( *lint1>*lint2)
      flag = 1;
   else if ( *lint1<*lint2 )
      flag = -1;
   else
   {
      length = *lint1;
      lint1 = lint1 + (*lint1);
      lint2 = lint2 + (*lint2);
      while ((flag==0)&&(length>0))
      {
         if (*lint1>*lint2)
            flag = 1;
         else if (*lint1<*lint2)
            flag = -1;
         length--;
         lint1--;
         lint2--;
      }
   }
   return flag;
}