// Program : 'SubKey' is generate subKey of IDEA, One subkey is 16 bits
// Author  : Avatar
// Date    : 98.05.01

void CShiftLeft(unsigned int sbit, unsigned long *csl);

void MakeSubKey(unsigned long *key, unsigned int *SKey)
{
   unsigned int sbit = 25;
   for (int i=0; i<7; i++) {
      for (int j=0; j<4; j++) {
         SKey[i*8+j*2+1] = (key[4-j]>>16)&0x0000ffff;
         SKey[i*8+j*2+2] = key[4-j]&0x0000ffff;
      }
      CShiftLeft(sbit, key);
   }
   *SKey = 52;
}