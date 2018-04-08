// Program : 'Compare' is to compare two integer
// Author  : Avatar
// Date    : 98.05.05

int compare(unsigned long *p1, unsigned long *p2)
{
   int flag = 0;

   if ( *p1>*p2 )
      flag = 1;
   else if ( *p1<*p2 )
      flag = -1;
   else
   {
      int cnt = *p1;
      while ( (cnt>0)&&(flag==0) )
      {
         if (p1[cnt]>p2[cnt])
            flag = 1;
         else if (p1[cnt]<p2[cnt])
            flag = -1;
         cnt--;
      }
   }
   return flag;
}