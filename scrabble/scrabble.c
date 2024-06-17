#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 == score2)
    {
        printf("Tie!\n");
    }
    else if (score1 > score2)
    {
        printf("Player 1 Wins!\n");
    }
    else
    {
        printf("Player 2 Wins!\n");
    }
}

int compute_score(string word)
{
    int scoreSum = 0;
    for (int i = 0, n = strlen(word); i < n; i++) // use a for loop to find and sum the score of each char in the string array
    {
        char tile = word[i]; // extract individual character element from the array
        if (isalpha(tile))   // evaluate using booleans whether its upper or lower or nonalpha
        {
            if (isupper(tile))
            {
                int tilePointUpper = POINTS[tile - 65]; // score for upper case
                scoreSum += tilePointUpper;
                // printf("%c", tile); //just to debug
                // printf("%i\n", tilePointUpper);
            }
            else
            {
                int tilePointLower = POINTS[tile - 97]; // score for lower case
                scoreSum += tilePointLower;
                // printf("%c", tile); // just to debug
                // printf("%i\n", tilePointLower);
            }
        }
        else
        {
            // printf("non alpha letter, zero points added\n"); //no points added to score, print message for debugging later
        }
    }
    return scoreSum;
    printf("scoring complete!\n");
}
