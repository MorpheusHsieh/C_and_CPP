// Program : Compare two integer, If p1 > p2 then flag =  1;
//                                If p1 = p2 then flag =  0;
//                                If p1 < p2 then flag = -1;
// Author  : Avatar
// Date    : 98.03.12
// Filename: Compare.cpp

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

int compare(unsigned long *p1, unsigned long *p2)
{
   long len;
   int flag = 0;
   if ( *p1>*p2 )
      flag = 1;
   else if ( *p1<*p2 )
      flag = -1;
   else
   {
      len = *p1;
      p1 = p1 + (*p1);
      p2 = p2 + (*p2);
      while ( (flag==0) && (len>0) )
      {
         if ( *p1>*p2 )
            flag = 1;
         else if ( *p1<*p2 )
            flag = -1;
         len --;
         p1 --;
         p2 --;
      }
   }
   return flag;
}
