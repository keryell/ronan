#ifndef _TT_TYPE_H_
#define _TT_TYPE_H_

typedef unsigned int TT_type;
typedef unsigned char uc;

typedef struct
{
	TT_type *ptrImage;
	unsigned int Height;
	unsigned int Width;
	unsigned int Stride;
	unsigned int Increment;
} TT_Image;

#endif
