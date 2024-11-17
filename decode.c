/*
   Author:afsal
   Organisation:EMERTXE
   Date:25/5/2024
   Usage:for decoding the content in output.bmp into our output text file(output.txt)
      */

#include <stdio.h>
#include<string.h>
/*enums used for better readability contains here*/
#include "types.h"
/*contains magic string*/
#include "common.h"
/*Prototypes of functions used in decode.c and structures used stores here*/
#include "decode.h"

 /*
      Get file pointer from input files and opens the input file
      Arguments:address of structure.
      Return value:e_success or e_failure
 
     */
Status open_input_file(DecodeInfo *decInfo)
{
    /*opens source image in read mode and stores its deatils in file pointer*/
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
      /*source image file not present error prints*/  
    if (decInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

        return e_failure;
    }
    /*if source file present e_success pass*/
 return e_success;
}

 /*
      Read command line argument and check the argument with needed extension
      Arguments:addresses of each arguments and structure address.
      Return value:e_success or e_failure
 
     */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    /*checks the file was with .bmp extension.if not error prints.if it is with .bmp name stores to the file pointer*/
    if((strstr(argv[2],".bmp"))==NULL)
    {
        printf("Usage: ./a.out -e input.bmp secret.txt output.bmp\nUsage -d output.bmp output.txt\n");
        return e_failure;
    }
    else
    {
        decInfo->src_image_fname=argv[2];
    }
    /*if source file present e_success pass*/
    return e_success;
}
 
/*
      Here decodes the caharacter contents in the output.bmp
      Arguments:address of string where data stores,size of string,structure address
      Return value:address of string where decoded content stores
 
     */

char* decode_string(char* ptr,int length,DecodeInfo* decInfo,int start)
{
    int i,j;
    /* decodes character content inside output.bmp */
    for(i=start;
	    i<length;
	    i++)
    {
        ptr[i]=0;
	/*decodes character by character*/
        fread((decInfo->storer),8,1,(decInfo->fptr_src_image));
        for(j=0;j<8;j++)
        {
            ptr[i]=ptr[i]|(decInfo->storer[j]&1)<<(7-j);
        }
    }
    ptr[i]='\0';
    /*return address of string which have decoded string*/
    return ptr;
}

 /*
      Here decodes the size of data like secret data size,extension size
      Arguments:addresses of structure
      Return value:size of data
 
     */

uint decode_size_data(DecodeInfo *decInfo)
{
int i;
uint val=0;
/*decodes size of content in the output.bmp*/
fread((decInfo->int_storer),8,4,(decInfo->fptr_src_image));
for(i=0;i<32;i++)
{
val=(val|((decInfo->int_storer[i]&1)<<(31-i)));
}
/*returns the size*/
return val;
}

/*
      Here decodes the magic string from output file
      Arguments:address of magic string,structure address
      Return value:e_success or e_failure
 
     */

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
/*offset position set to 54th byte*/
fseek(decInfo->fptr_src_image,54,SEEK_SET);
/*creates a string with length of magic string+1*/
char str[strlen(magic_string)+1];
/*functiion call where char decode*/
decode_string(str,strlen(magic_string),decInfo,0);

/*if decoded string matches return success otherwise failure*/
if(strcmp(str,magic_string)==0)
{
return e_success;
}
else
    return e_failure;

}

 /*
      Decodes file extension size
      Arguments:addresses structure address.
      Return value:e_success or e_failure
 
     */

Status decode_secret_file_extn_size(DecodeInfo* decInfo)
{
    /**with help of size decoder decodes size*/
decInfo->file_ext_size=decode_size_data(decInfo); 
/*check size contains any value*/
        if(decInfo->file_ext_size>0)
        {
        return e_success;
        }
        else
            return e_failure;

}

 /*
      Decodes file extension data
      Arguments:addresses of each arguments.
      Return value:e_success or e_failure
 
     */

Status decode_secret_file_extn(unsigned int size, DecodeInfo *decInfo)
{
    /*making the string for content storing*/
char str[size+1];
decode_string(str,size,decInfo,0);
long int val=-(8*(long int)size);
/*creating string for file name storing*/
char file[6+(size+1)];
strcpy(file,"output");
/*sets address where we need to get the content*/
fseek(decInfo->fptr_src_image,val,SEEK_CUR);
    decode_string(file,(6+size),decInfo,6);

/*VERY IMP.never copy anything to string address from any other st    ring address.just give address of the string(which have value) to other string. decInfo->secret_te    xt_fname=file  or  decInfo->secret_text_fname=argv[3];*/
decInfo->secret_text_fname=file;

/*if successfully file name stores create an output.txt file*/
decInfo->fptr_secret_text = fopen(decInfo->secret_text_fname, "w");

return e_success;

}

 /*
      Decodes secret file size
      Arguments:addresses of each arguments.
      Return value:e_success or e_failure
 
     */

Status decode_secret_file_size(DecodeInfo* decInfo)
{
    /*stores the file size */
    decInfo->file_size=decode_size_data(decInfo);
    /*checks file size greater than 0*/
        if(decInfo->file_size>0)
        {
        return e_success;
        }
        else
            return e_failure;

}

 /*
      Decodes secret file data into secret file
      Arguments:addresses of each arguments.
      Return value:e_success or e_failure
 
     */

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    /*creates string where data stores*/
char str[decInfo->file_size+1];
/*decodes content into string*/
decode_string(str,decInfo->file_size,decInfo,0);
/*stores the string into output file*/
fwrite(str,(sizeof(str)-1),1,decInfo->fptr_secret_text);
return e_success;
}

 /*
      Function call of every Operation.checking each operation successfull or no
      Arguments:addresses of each arguments.
      Return value:e_success or e_failure
 
     */

Status do_decoding(DecodeInfo *decInfo)
{
    if(open_input_file(decInfo)==e_success)
    {
        printf("ALL FILES OPENED SUCCESSFULLY\n\n");

                if(decode_magic_string(MAGIC_STRING,decInfo)==e_success)
                {
                    printf("MAGIC STRING STRING MATCHES\n\n");
                   if(decode_secret_file_extn_size(decInfo)==e_success)
                    {
                        printf("SECRET FILE EXTENSION SIZE DECODED SUCCESSFULLY\n\n");

                        if(decode_secret_file_extn(decInfo->file_ext_size,decInfo)==e_success)
                        {
                            printf("SECRET FILE EXTENSION FIND OUT AND OUTPUT FILE NAME CREATED\n\n");
                        if(decode_secret_file_size(decInfo)==e_success)
                        {
                            printf("SECRET FILE SIZE DECODED SUCCESSFULLY\n\n");
                            if(decode_secret_file_data(decInfo)==e_success)
                            {
                                printf("SECRET FILE DECODED SUCCESSFULLY\n\n");
                                    return e_success;
                            }
                            else
                            {
                                printf("SECRET FILE DECODING FAILED\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("SECRET FILE SIZE DECODING FAILED\n");
                            return e_failure;
                        }
                        }
                        else
                        {
                        printf("OUTPUT FILE CREATION FAILED\n");
                        }
                    }
                    else
                    {
                        printf("SECRET FILE EXTENSION SIZE IS NOT THERE\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("MAGIC STRING NOT MATCHING\n");
                    return e_failure;
                }

            }

    else
    {
        printf("FILES OPENING FAILED\n");
        return e_failure;
    }
}
