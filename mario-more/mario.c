#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //prompt user for positive integer to make height of pyramid
    int pyHeight;
    do
    {
        pyHeight = get_int("Pyramid Height: ");
    }
    while (pyHeight < 1 || pyHeight > 8);

    //build each level of pyramid
    for (int i = 1; i < pyHeight + 1; i++){
        int spaceCount = pyHeight - i;
       for (int j = 0; j < i; j++){
        while (spaceCount > 0)
        {
            printf(" ");
            --spaceCount;
        }
        printf("#");
       }
       printf("  ");
       for (int k = 0; k < i; k++){
        printf("#");
       }
    printf("\n");
    }
     return 0;
}







        // //loop for hashmarks on left side of pyramid
        // for (int j = 0; j < i + 1; j++){
        //     levelInsert[7 - j] = '#';
        // }
        // //loop for hashmarks on right side of pyramid
        // for (int k = 0; k < i + 1; k++){
        //     levelInsert[10 + k] = '#';
        //insert constructed level with hashmarks into 2d array variable
        // for (int m = 0; m < 17; m++){
        //     pyramid[i][m] = levelInsert[m];
        // }



    //print pyramid shape from 2d array variable
    // for (int i = 0; i < pyHeight; i++){
    //     for (int j = 0; j < 17; j++){
    //         printf("%c", pyramid[i][j]);
    //     }
    //     printf("\n");

    // }


