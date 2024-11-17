/*
Author:afsal
Organisation:EMERTXE
Date:25/5/2024
Usage:for encoding the content in secret.txt into our input file(beautiful.bmp)
   */

#include <stdio.h>
#include<string.h>
/*Prototypes of functions used in encode.c and structures used stores here*/
#include "encode.h"
/*enums used for better readability contains here*/
#include "types.h"
/*contains magic string*/
#include "common.h"
#include<stdlib.h>
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    /* Seek to 18th byte*/
    fseek(fptr_image, 18, SEEK_SET);

    /* Read the width (an int)*/
    fread(&width, sizeof(int), 1, fptr_image);

    /* Read the height (an int)*/
    fread(&height, sizeof(int), 1, fptr_image);

    /* Return image capacity*/
    return width * height * 3;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    /* Src Image file*/
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    /* Do Error handling*/
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    /* Secret file */
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    /* Do Error handling*/
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    /* Stego Image file*/
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    /* Do Error handling */
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    /* No failure return e_success */
    return e_success;
}
 /*
    Read command line arguments and check the arguments  are with needed extension
    Arguments:addresses of each arguments.
    Return value:e_success or e_failure

   */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    /* Checking 2nd argument is .bmp or not */
    if((strstr(argv[2],".bmp"))==NULL)
    {
        printf("Usage: ./a.out -e input.bmp secret.txt output.bmp\nUsage -d output.bmp output.txt\n");
        return e_failure;
    }
    else
    {
        encInfo->src_image_fname=argv[2];

    }

    /* Checking 3rd argument is .txt or not */
    if((strstr(argv[3],".txt"))==NULL)
    {
         printf("Usage: ./a.out -e input.bmp secret.txt output.bmp\nUsage -d output.bmp output.txt\n");
         return e_failure;
    }
    else
    {
        encInfo->secret_fname=argv[3];

        encInfo->extn_secret_file=strstr(encInfo->secret_fname,".");

    }

    /* Checking 4th argument is .bmp or not.if no argument create a file with .bmp */
    if(argv[4]!=NULL)
    {
        if((strstr(argv[4],".bmp"))==NULL)
        {
             printf("Usage: ./a.out -e input.bmp secret.txt output.bmp\nUsage -d output.bmp output.txt\n");
            return e_failure;
        }
        else
        {
            encInfo->stego_image_fname=argv[4];
        }
    }
    else
    {
        encInfo->stego_image_fname="output.bmp";
    }
    /*In case of success pass it*/
    return e_success;
}

 /*
    For getting size of secret file
    Arguments:file pointer of secret file.
    Return value:size of secret file

   */
uint get_file_size(FILE* secret)
{
    char ch;
    uint size=0;
    /*Finds out the no.of characters present in secret file*/
    while((ch=fgetc(secret))!=EOF)
        size++;
    /*Return size of secret file*/
    return size;
}

/*
   Checking capacity of beautiful.bmp file is enough for storing secret data
   Arguments:address of structure.
   Return value:e_success or e_failure.

   */
Status check_capacity(EncodeInfo *encInfo)
{
    /*Finds the image capacity of input file and size off content needs to encode */
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);

    uint size=((encInfo->size_secret_file)*8)+(strlen(MAGIC_STRING)*8)+(strlen(encInfo->extn_secret_file)*8)+(2*(sizeof(int)));
    encInfo->size_secret_extn=strlen(encInfo->extn_secret_file);
     /*Checks the size is enough for storing or not*/
    if((encInfo->size_secret_file)>0)
    {
        if((encInfo->image_capacity)>size)
            return e_success;
        else
            return e_failure;
    }
    else
    {
        printf("SECRET FILE IS EMPTY\n");
        return e_failure;
    }

}

/*
   for encoding the given data into last byte of image
   Arguments:data needed to be encoded,size of data,file pointers of image and secret file
   Return value:e_success or e_failure.

   */
Status encode_data_to_image(char* data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    EncodeInfo* encoInfo;
    /* Reading source image data and encoding content inside it*/
    for(int i=0;
            i<size;
            i++)
    {
        fread(encoInfo->image_data,8,1,fptr_src_image);
        encode_byte_to_lsb(data[i],encoInfo->image_data);
        fwrite(encoInfo->image_data,8,1,fptr_stego_image);
    }
    return e_success;
}

/*
   Encode each byte of data into image last byte
   Arguments:1 byte of secret data needed to encode,8 byte of image data
   Return value:e_success or e_failure.

   */

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /*Storing 8byte from source image and encoding 1 byte data into it*/
    for(int i=0;i<8;i++)
    {
        image_buffer[i]=(image_buffer[i]&(~1))|((data>>(7-i))&1);
    }
    return e_success;
}

/*
   .bmp file header copies to output image
   Arguments:file pointer of input image and output image
   Return value:e_success or e_failure.

   */

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char * ptr;
    rewind(fptr_src_image);
    /* reads 54 byte from source image and stores it in output image*/
    fread(ptr,54,1,fptr_src_image);
    fwrite(ptr,54,1,fptr_dest_image);
    /*Return if succesfully copied*/
    return e_success;

}

/*
   For encoding data in magic string into output image
   Arguments:address of string where magic string stores
   Return value:e_success or e_failure.

   */

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    /*calling function which will encode character data into source image*/
    encode_data_to_image(magic_string,strlen(magic_string),(encInfo->fptr_src_image),(encInfo->fptr_stego_image));
    /*return success if it encoded successfully*/
    return e_success;

}

/*
   For encoding secret file extension data into output image
   Arguments:address of string where file extension stores
   Return value:e_success or e_failure.

   */
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    /*calling function which will encode character data into source image*/
    encode_data_to_image(file_extn,strlen(file_extn),(encInfo->fptr_src_image),(encInfo->fptr_stego_image));
    /*return success if it encoded successfully*/
    return e_success;

}

/*
   For encoding size wise datas (size of secret file extension,size of secret file) into output image
   Arguments:size of data
   Return value:e_success or e_failure.

   */
Status encode_secret_size(long file_size, EncodeInfo *encInfo)
{
    int i;
    char arr[32];
    /*Reads 32 byte data from input image into an array*/
    fread(arr,32,1,(encInfo->fptr_src_image));
    /*copies each bit  into the array after encode */
    for(i=0;i<32;i++)
    {
        arr[i]=(arr[i]&(~1))|((file_size>>(32-1-i))&1);
    }
    /*Writes the encoded array into output image */
    fwrite(arr,32,1,(encInfo->fptr_stego_image));
    return e_success;
}

/*
   For encoding the secret data into input image
   Arguments:address of structure
   Return value:e_success or e_failure.

   */
Status encode_secret_file_data(EncodeInfo *encInfo)
{

   rewind(encInfo->fptr_secret);

   char *str=(char*)calloc(encInfo->size_secret_file,sizeof(char));
   /*reads secret file data into a string*/
    fread(str,encInfo->size_secret_file,1,encInfo->fptr_secret);
     printf(" \n");
     /*calls function for encoding character data*/
    encode_data_to_image(str,encInfo->size_secret_file,(encInfo->fptr_src_image),(encInfo->fptr_stego_image));
    /*Making string free*/
    free(str);
    return e_success;
}

/*
   For copying the remaining data in input file
   Arguments:file pointer of input image and output image
   Return value:e_success or e_failure.

   */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest,EncodeInfo *encInfo)
{
    char ch;
    char* ptr=&ch;
    /*Reading char by char from input file upto EOF and stores it in output file*/
    while(fread(ptr,1,1,fptr_src)!=0)
    {
        fwrite(ptr,1,1,fptr_dest);
    }
     /*Closing all file pointers*/
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);

    return e_success;
}

/*
   Function call of every Operation.checking each operation successfull or not
   Arguments:address of structure
   Return value:e_success or e_failure.

   */
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo)==e_success)
    {
        printf("ALL FILES OPENED SUCCESSFULLY\n\n");
        if(check_capacity(encInfo)==e_success)
        {
            printf("ENOUGH RGB BYTE PRESENT FOR ENCODING\n\n");
            if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
            {
                printf("54 BYTES COPIED SUCCESFULLY\n\n");

                if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
                {
                    printf("MAGIC STRING STORED SUCCESSFULLY\n\n");
                    if(encode_secret_size(encInfo->size_secret_extn,encInfo)==e_success)
                    {
                        printf("SECRET FILE EXTENSION SIZE STORED SUCCESSFULLY\n\n");
                        if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)
                        {
                            printf("SECRET FILE EXTENSION STORED SUCCESSFULLY\n\n");
                            if(encode_secret_size(encInfo->size_secret_file,encInfo)==e_success)
                            {
                                printf("SECRET FILE SIZE ENCODED SUCCESSFULLY\n");
                                if(encode_secret_file_data(encInfo)==e_success)
                                {
                                    printf("SECRET FILE ENCODED SUCCESSFULLY\n\n");
                                    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo)==e_success)
                                    {
                                        printf("REMAINING DATA ENCODED SUCCESSFULLY\n\n");
                                        return e_success;
                                    }
                                    else
                                    {
                                        printf("REMAINING DATA ENCODING FAILED\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("SECRET FILE ENCODING FAILED\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("SECRET FILE SIZE ENCODING FAILED\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("SECRET FILE EXTENSION STORING FAILED\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("SECRET FILE EXTENSION SIZE STORING FAILD\n");
                    }
                }
                else
                {
                    printf("MAGIC STRING STORING FAILED\n");
                    return e_failure;
                }

            }
            else
                printf("54 BYTES COPYING FAILED\n");
            return e_failure;

        }
        else
            printf("ENOUGH RGB BYTES ARE NOT PRESENT\n");
        return e_failure;

    }
    else
    {
        printf("FILES OPENING FAILED\n");
        return e_failure;
    }
}
