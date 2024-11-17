#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* * Structure to store information required for
 * decode secret file from output image
 */


typedef struct _DecodeInfo
{
    /* Output Image info */
  
    char *src_image_fname;
    FILE *fptr_src_image;
    char storer[8];
    char int_storer[32];

    /* Secret text storing file Info */
    char *secret_text_fname;
    FILE *fptr_secret_text;
    unsigned int file_ext_size;
    unsigned int file_size;

} DecodeInfo;

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_input_file(DecodeInfo *decInfo);

/*Gives size of decoded data*/
uint decode_size_data(DecodeInfo *decInfo);

/*gives decoded character arrays*/
char* decode_string(char* ptr,int length,DecodeInfo* decInfo,int start);


/* decodes magic string*/
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/* Decode secret file extenstion size */
Status decode_secret_file_extn_size( DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(unsigned int size, DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);


#endif
