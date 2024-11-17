/*
Author:afsal
Organisation:EMERTXE
Date:25/5/2024
Usage:for calling functions for encoding and decoding from this main function
   */


#include <stdio.h>
#include <string.h>
/*Prototypes of functions used in encode.c and structures used stores here*/
#include "encode.h"
/*Prototypes of functions used in decode.c and structures used stores here*/
#include "decode.h"
/*enums used for better readability contains here*/
#include "types.h"
//OperationType check_operation_type(char* argv[]);

int main(int argc,char* argv[])
{
    /*making a variable with enum type*/
    OperationType type;
    /*checks what operation user prefer encode or decodea*/
    if((type=check_operation_type(argv))==e_encode)//check user entered -e for encoding
    {
    EncodeInfo encInfo;
    /*checks enough command line argument present or not*/
	if((argc==4)||(argc==5))            
	{
	    /*checks arguments are valuable or not*/
	    if((read_and_validate_encode_args(argv,&encInfo))==e_success)
	    {
		/*encoding starts here.checks encoded successfully or not*/
		if(do_encoding(&encInfo)==e_success)
		{
		    printf("ENCODED SUCCESSFULLY\n\n");
		}
		else
		{
		    printf("ENCODING FAILED\n");
		}		 
	    }
	    else
	    {
		printf("SO ENCODING FAILED\n");
	    }
	}
	else
	{
	    printf("Usage: ./a.out -e input.bmp secret.txt output.bmp\nUsage -d output.bmp output.txt\n");
	}
    }
    else if(type==e_decode)
    { 
	DecodeInfo decInfo;
	/*checks enough command line argument present or not*/
	if((argc==3)||(argc==4))             
	{
             /*checks arguments are valuable or not*/
	    if((read_and_validate_decode_args(argv,&decInfo))==e_success)
	    {
		/*encoding starts here.checks encoded successfully or not*/
		if(do_decoding(&decInfo)==e_success)
		{
		    printf("DECODED SUCCESSFULLY\n\n");
		}
		else
		{
		    printf("DECODING FAILED\n");
		}		 
	    }
	    else
	    {
		printf("SO DECODING FAILED\n");
	    }
	}
	else
	{
	    printf("Usage: ./a.out -e input.bmp secret.txt output.bmp\nUsage -d output.bmp output.txt\n");
	}
	
	
    }
    else if(type==e_unsupported)
    {
	printf("Usage: ./a.out -e input.bmp secret.txt output.bmp\nUsage -d output.bmp output.txt\n");
    }


    return 0;
}

/*
    finds out what operation user preefer
    Arguments:address of command line argument.
    Return value:value defines the operation 

 */
OperationType check_operation_type(char* argv[])
{
    /*checks if user provided any input*/
    if(argv[1]==NULL)
	printf("Usage: ./a.out -e input.bmp secret.txt output.bmp\nUsage -d output.bmp output.txt\n");
    else if((strcmp(argv[1],"-e"))==0)
	return e_encode;
    else if((strcmp(argv[1],"-d"))==0)
	return e_decode;
    else
	return e_unsupported;

}
