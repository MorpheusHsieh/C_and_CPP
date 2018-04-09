/* =======================================================================
		FLIPS1.H
		Created by Morpheus    Jun-16-98
   ======================================================================= */

#define M2        0x01
#define M256		0x02
#define C16			0x11
#define CP			0x12
#define CH			0x13
#define CT			0x14
#define CE			0x15

#define TRUE			1
#define FALSE			0

#define MAX_PALETTE_SIZE	256

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

typedef struct
{
  byte version;
  byte mode;
  byte id;
  word width;			/* lower byte first */
  word height;
  byte reserved[25];
} META_PREFIX;

typedef struct
{
  byte R;
  byte G;
  byte B;
} COLOR_INDEX;

typedef struct
{
  word size;
  COLOR_INDEX color[MAX_PALETTE_SIZE];
} COLOR_MAP;

