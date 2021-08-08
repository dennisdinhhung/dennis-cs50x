#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[])
{   
    typedef uint8_t BYTE; //define an unsigned 8 bit int as BYTE

    //argument error checker
    if (argc != 2){
        printf("Usage: ./recover image\n");
        return 1;
    }

    //TODO: Open memory card

    FILE *input = fopen(argv[1], "r");
    if (input == NULL){
        printf("Could not open file!\n");
        return 1;
    }
    FILE *output;

    bool found = false;
    int count_jpg=0;
    BYTE chunk[512];
    char filename[8];

    while(fread(chunk, sizeof(chunk), 1, input)){
        //TODO: Look for JPEG header data
            //* JPEG starts with the header of: 0xff 0xd8 0xff and 0xe? 
        if (chunk[0] == 0xff && chunk[1] == 0xd8 && chunk[2] == 0xff && (chunk[3] & 0xf0) == 0xe0){
            //TODO: Open a new JPEG file
            //* The jpg starts at 000.jpg then increment for the next jpgs
            if(count_jpg == 0){
                sprintf(filename, "%03i.jpg", count_jpg);
                output = fopen(filename, "w");
                fwrite(chunk, sizeof(chunk), 1, output);
                count_jpg++;
                found = true;
            }
            else{
                fclose(output);
                sprintf(filename, "%03i.jpg", count_jpg);
                output = fopen(filename, "w");
                fwrite(chunk, sizeof(chunk), 1, output);
                count_jpg++;
            }
        }
        else if(found){
            fwrite(chunk, sizeof(chunk), 1, output);
        } 
    }

    fclose(input);
    fclose(output);
    return 0;
}