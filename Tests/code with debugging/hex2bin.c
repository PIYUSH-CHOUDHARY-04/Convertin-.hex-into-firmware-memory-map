#include "hex2bin.h"

uint8_t ParsedData[RECORD_DATA_MAX_SIZE/2]={0};

ParsedRecord RecordX={0};
RefStruct ref={0};


void printStruct(void){
    printf("current record info : \n");
    printf("data_addr : %#x\ncurr_record : %ld\nrecord_type : %d\nCheckSum : %d\ndata_len : %d\n,ptr_parsed : %p\n",RecordX.data_addr,RecordX.curr_record,RecordX.record_type,RecordX.CheckSum,RecordX.data_len,RecordX.ptr_parsed);
}

/**
 * @brief Routine to get total record count, this routine just count the Record mark, thus it's end user's responsibility to pass a valid hex file else it will just count the ':' in the file.
 * @param FILE* fptr passes the FILE* pointer of the opened .hex file.
 * @retval uint64_t returns the total record count in the file, note that for any architecture, number of records can be more than the number of bytes in the addressable memory.
 */
uint64_t GetRecordCount(FILE* fptr){
    /* getting the current buffer pointer for the case when the passed fptr doesn't have the buffer pointer pointing to initial byte. */
    fpos_t init_ptr;
    fgetpos(fptr, &init_ptr);
    
    /* Ensuring the pointer in opened file stream points to first byte of the file. */
    rewind(fptr);
    uint64_t count=0;
    int char_read=0;
    
    while((char_read=fgetc(fptr))!=EOF){
        if(char_read==':'){
            count++;        
        }
        continue;
    }
    fsetpos(fptr, &init_ptr);
    return count;
}

/**
 * @brief Rountine to convert the 2 bytes of text into a single hex byte.
 * @param uint8_t b1 passes the value of byte 1
 * @param uint8_t b2 passes the value of byte 2
 * @retval uint8_t returns the combined value of b1 and b2.
 */
uint8_t Hextext2Byte(uint8_t b1, uint8_t b2){
    printf("executing Hextext2Byte()...\n");
    printf("b1 : %d | b2 : %d\n",b1,b2);
    b1-=48;
    b2-=48;
    if(b1>9){
        b1-=7;
    }
    if(b2>9){
        b2-=7;
    }
    printf("b1 : %d | b2 : %d\n",b1,b2);
    return ((b1<<4)|(b2)); 
}

#if __SYS_ARCH == ARCH_8_BIT
/**
 * @brief Routine to parse ARCH_8_BIT
 * @param int agrc passes the number of args received via command line.
 * @param char** argv passes the pointer to the array containing pointer to the null terminated C strings passed via command line.
 * @retval void
 */
void ARCH_8_BIT_parser(int argc, char** argv);
#endif /* __SYS_ARCH == ARCH_8_BIT */

#if __SYS_ARCH == ARCH_16_BIT
/**
 * @brief Routine to parse ARCH_8_BIT
 * @param int agrc passes the number of args received via command line.
 * @param char** argv passes the pointer to the array containing pointer to the null terminated C strings passed via command line.
 * @retval void
 */
void ARCH_16_BIT_parser(int argc, char** argv);
#endif /* __SYS_ARCH == ARCH_16_BIT */

#if __SYS_ARCH == ARCH_32_BIT
/**
 * @brief Routine to parse ARCH_8_BIT
 * @param int agrc passes the number of args received via command line.
 * @param char** argv passes the pointer to the array containing pointer to the null terminated C strings passed via command line.
 * @retval void
 */
void ARCH_32_BIT_parser(int argc, char** argv){
    RecordX.ptr_parsed=ParsedData;
    printf("Size of data_addr : %ld\n",sizeof(RecordX.data_addr));

    for(uint8_t i=0;i<argc-1;i++){
        FILE* fptr_hex=fopen(argv[i+1],"r");
        if(fptr_hex==NULL){
            printf("Unable to open file : %s\nCheck whether the file name you passed exists\n",argv[i+1]);
        }

        int file_name_len=strlen(argv[i+1]);
        char* change_extension = (char*)malloc(sizeof(char)*(file_name_len+1)); /* +1 for including null terminator. */
        /* copying the file name to this array till the "." just before extension name like .hex/.bin */
        memcpy(change_extension,argv[i+1],file_name_len-3);
        change_extension[file_name_len-3]='b';
        change_extension[file_name_len-2]='i';
        change_extension[file_name_len-1]='n';
        change_extension[file_name_len] ='\0';
        /* opening .bin file in binary write mode. */
        FILE* fptr_bin=fopen(change_extension,"wb");
        if(fptr_bin==NULL){
            printf("Unable to create file : %s\nCheck the directory permissions or system resources.\n",change_extension);
        }
        printf("Total Records found : %ld\n", GetRecordCount(fptr_hex));


        /* reading records one by one */
        int iter=0;
        while(fgetc(fptr_hex)==':'){
            printf("iter : %d\n",iter);
            iter++;
            printf("Parsing Record %ld\n",RecordX.curr_record);
            
            uint8_t b1=(uint8_t)fgetc(fptr_hex);
            uint8_t b2=(uint8_t)fgetc(fptr_hex);
            uint8_t b3=0;
            uint8_t b4=0;

            printf("b1 : %d | b2 : %d\n",b1,b2);
            RecordX.data_len=Hextext2Byte(b1, b2);     /* typecasting int to uint8_t since fgetc() returns values between 0-255 on success, on failure returns -1 which 
                                                                                                   is taken care in while() condition check. */
            printf("RecordX.data_len : %d\n",RecordX.data_len);
            /* getting the address stored into the address field of the current record,  */
            b1=(uint8_t)fgetc(fptr_hex);
            b2=(uint8_t)fgetc(fptr_hex);

            b3=Hextext2Byte(b1, b2);
    
            b1=(uint8_t)fgetc(fptr_hex);
            b2=(uint8_t)fgetc(fptr_hex);
            
            b4=Hextext2Byte(b1, b2);
            
            uint16_t temp_addr=( (uint16_t)b3<<8 | (uint16_t)b4 );
            printf("temp_addr : %d\n",temp_addr);
            
            b1=(uint8_t)fgetc(fptr_hex);
            b2=(uint8_t)fgetc(fptr_hex);
            printf("b1 : %d | b2 : %d\n",b1,b2);
    
            RecordX.record_type=Hextext2Byte(b1, b2);
           
            printf("RecordX.record_type : %d\n",RecordX.record_type);

            if(RecordX.record_type == REC_TYPE_ELAR){ /* Searching for frame of record type ELAR, thus it will have a standard structure as described in the corresponding header 
                                                                                        file. */
                printf("In if block of REC_TYPE_ELAR\n");
                /* Since the  record is of ELAR type, thus need to fetch next 4 bytes (these 4 bytes will be of data field.) */   
                b1=(uint8_t)fgetc(fptr_hex);
                b2=(uint8_t)fgetc(fptr_hex);

                b3=Hextext2Byte(b1, b2);
    
                b1=(uint8_t)fgetc(fptr_hex);
                b2=(uint8_t)fgetc(fptr_hex);
            
                b4=Hextext2Byte(b1, b2);
                
                temp_addr=( (uint16_t)b3<<8 | (uint16_t)b4 );
                printf("temp_addr : %#x\n",temp_addr);

                if(ref.flag_higher == 0x00){
                    /* according to the structure of this record_type, need to fetch next 4 bytes from text file to get the higher 16 bit offset. */
                    ref.first_data_addr_higher=temp_addr;
                    ref.flag_higher=0x01;  
                }
                
                /* Clearing only the upper 16 bits, the entire if block will be executed whenever the ELAR record is found thus its necessary to update the upper 16 bits of the offset. */
                /* Thus ANDing with (uint16_t)(0x00FF) */
                (RecordX.data_addr)&=(uint32_t)(0x0000FFFF);
                (RecordX.data_addr)|=(((uint32_t)temp_addr)<<16); /* setting upper 16 bits. */

                /* data field for the record of type REC_TYPE_ELAR is of 4 bytes in text file, thus moving to next field which is checksum, getting checksum and eating up enter character, carriage return character
                   so that next iteration of the while() loop will satisfy its condition i.e. buffer pointer points to ':' field/byte of the next record. */  
                b1=(uint8_t)fgetc(fptr_hex);
                b2=(uint8_t)fgetc(fptr_hex);
  
                (RecordX.CheckSum)=Hextext2Byte(b1, b2);
                b1=fgetc(fptr_hex);
                printf("after record char : %d %c\n",b1,b1);
                printStruct();
                (RecordX.curr_record)++;
                
                b1=(uint8_t)fgetc(fptr_hex);
                printf("CHAR : %d %c\n",b1,b1);
                printf("upper 16 : %#x | lower 16 : %#x\n",ref.first_data_addr_higher, ref.first_data_addr_lower);
                continue;       
            }
            else
            if(RecordX.record_type == REC_TYPE_DATA){ /* getting the refrence value of offset for lower 16 bits by reading first data record.. */
                /* Since the record is of type REC_TYPE_DATA, thus no need to fetch the next byte for now, temp_addr will be holding the lower 16 bits of 32 bit address. */
                printf("In if block of REC_TYPE_DATA\n");
                if(ref.flag_lower == 0x00){
                    ref.first_data_addr_lower=temp_addr;
                    ref.flag_lower=0x01;
                }
                
                /* Clearing the lower 16 bits of the offset since each record of REC_TYPE_DATA will be writing the data to new address with different lower 16 bits of the offset. */
                (RecordX.data_addr)&=(uint32_t)(0xFFFF0000);
                (RecordX.data_addr)|=(temp_addr);
                /* need to parse the data and store it into the array pointed by ptr_parsed attribute of the RecordX structure. */
                for(uint8_t k=0;k<RecordX.data_len;k++){
                    b1=(uint8_t)fgetc(fptr_hex);
                    b2=(uint8_t)fgetc(fptr_hex);
                    (RecordX.ptr_parsed)[k]=Hextext2Byte(b1, b2);
                }  
                /* Now parsing checksum and eating up the enter character. */
                b1=(uint8_t)fgetc(fptr_hex);
                b2=(uint8_t)fgetc(fptr_hex);

                RecordX.CheckSum=Hextext2Byte(b1, b2);
                fgetc(fptr_hex);
                fgetc(fptr_hex);
                /* Now we've the entire data for the current record, thus need to write this data at the address governed by data_addr member of the RecordX structure, but anyway, in the .bin file,
                   the data will be written from 0th byte and its programmer's responsibility to flash the firmware at required address according to the underlying micro-controller's data sheet. */
                /* But we atleast need to write the data in memory w.r.t initial refrence offset in orde to have correct firmware layout in the resultant .bin file. */
                /* Writing the memory map   */
                printf("Writing the memory map of the firmware.\n");
                long diff=(RecordX.data_addr) - (((uint32_t)(ref.first_data_addr_higher)<<16)|(uint32_t)(ref.first_data_addr_lower));
                printf("OFFSET_DIFF : %ld \n",diff);
                fseek(fptr_bin, diff, SEEK_SET);
                fwrite(RecordX.ptr_parsed, sizeof(uint8_t), RecordX.data_len, fptr_bin);
                printStruct();                
                (RecordX.curr_record)++;
                printf("upper 16 : %#x | lower 16 : %#x\n",ref.first_data_addr_higher, ref.first_data_addr_lower);
                
                
                
            }
            else
            if(RecordX.record_type == REC_TYPE_EOF){
                /* This type of record doesn't stores any information about data and address of the data but only tells that the records are over in the current hex file. */
                printf("In if block of REC_TYPE_EOF\n");
                RecordX.data_addr=0x00; /* marking the entire field as 0 since entire file has been parsed */                
                /* getting checksum */
                b1=(uint8_t)fgetc(fptr_hex);
                b2=(uint8_t)fgetc(fptr_hex);

                RecordX.CheckSum=Hextext2Byte(b1, b2);
                /* Exiting while loop */
                printStruct();
                (RecordX.curr_record)++;
                printf("upper 16 : %#x | lower 16 : %#x\n",ref.first_data_addr_higher, ref.first_data_addr_lower);
                
                
                break;
            }
            else{
                /* Need to skip that record since we just need the final firmware layout, other records may just set the IP/PC to specific value after bootime firmware update. */
                /* Eating up all the chars until '\n' is not encountered */
                printf("In if block of REC_TYPE_remaining\n");
                do{
                    fgetc(fptr_hex);
                }while(fgetc(fptr_hex)!='\n');
                fgetc(fptr_hex);
                printStruct();
                (RecordX.curr_record)++;
                printf("upper 16 : %#x | lower 16 : %#x\n",ref.first_data_addr_higher, ref.first_data_addr_lower);
                
            }
        
        }
        free(change_extension);
        fclose(fptr_bin);
        fclose(fptr_hex);
    }

}
#endif /* __SYS_ARCH == ARCH_32_BIT */




void main(int argc, char** argv){   /* Pass the .hex file path */
#if __SYS_ARCH == ARCH_8_BIT
    ARCH_8_BIT_parser(argc, argv);
#endif /* __SYS_ARCH == ARCH_8_BIT */    

#if __SYS_ARCH == ARCH_16_BIT
    ARCH_16_BIT_parser(argc, argv);
#endif /* __SYS_ARCH == ARCH_16_BIT */

#if __SYS_ARCH == ARCH_32_BIT
    ARCH_32_BIT_parser(argc, argv);
#endif /* __SYS_ARCH == ARCH_8_BIT */
    
    printf("Total files parsed : %d\n",argc-1);
    printf("Parsed Files : \n");
    for(uint8_t j=0;j<argc-1;j++){
        printf(">> %s\n",argv[j+1]);
    }
    
}






