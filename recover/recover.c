#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <math.h>
#include <stdint.h>

#define FAT 512
typedef uint8_t BYTE;


int main(int argc, char *argv[])
{
    // check for just one command line argument, the file name to be read...
    if (argc != 2)
    {
        printf("Usage: ./recover input_file\n");
        return 1;
    }

    // Open input file
    FILE *card = fopen(argv[1], "r");


    if (card == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        fclose(card);
        return 1;
    }
    // create buffer to read data
    uint8_t buffer[FAT];
    int alreadyOpen = 0;
    int jpegCounter = 0;

    //declare output file outside of while loop
    FILE *img = NULL;
    char filename[8];

    //use a while loop to make sure we still have a byte[FAT] chunk to read off the card. will iterate until we've read through the end of card
    while (fread(buffer, 1, FAT, card) == FAT)
    {
        // Read the first four bytes of every FAT to see if they match the JPEG signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (alreadyOpen == 0)
            {
                sprintf(filename, "%03i.jpg", jpegCounter);
                img = fopen(filename, "w");
                if (img == NULL)
                {
                    printf("Could not create %s.\n", filename);
                    return 2;
                }

                // Write data to outfile, copying whole FAT bytes chunk
                fwrite(buffer, 1, FAT, img);
                alreadyOpen = 1;
            }
            else if (alreadyOpen == 1)
            {
            // this else case means that we've run into another signature while copying a jpeg, so now its time to reset!
            jpegCounter++;
            fclose(img);
            sprintf(filename, "%03i.jpg", jpegCounter);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                printf("Could not create %s.\n", filename);
                return 2;
            }

            // Write data to outfile, copying whole FAT bytes chunk
            fwrite(buffer, 1, FAT, img);
            }
        }
        else
        {
            if (alreadyOpen == 1)
            {
                fwrite(buffer, 1, FAT, img);
            }
        }

    }
    fclose(img);
    fclose(card);
    // this is end of the WHILE loop for reading from memory card

}

