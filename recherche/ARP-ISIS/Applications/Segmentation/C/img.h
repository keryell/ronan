#ifndef _IMG_H_
#define _IMG_H_

TT_Image *read_image_file( const char *FileName );
void write_image_file( const char *FileName, TT_Image *TImage );
void print_image( TT_Image *Timage );

#endif
