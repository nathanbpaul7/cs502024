#include <cs50.h>
#include <stdio.h>

int get_digits(void);
int calculate_sum1(long cardNumber);
int calculate_sum2(long cardNumber);
int number_length(long cardNumber);

int main(void)
{
    long cardNumber = get_long("Number: ");   // prompt for credit card number
    int sum1 = calculate_sum1(cardNumber); // get first sum of first set of digits * 2 (with rule for double digit products)
    int sum2 = calculate_sum2(cardNumber); // get next sum of second set of digits

    // determine if card is valid
    int total = sum1 + sum2;

    if (total % 10 == 0) //then it's valid!  let's find out what kind of card it is using its length and identifying numbers
    {
        int cardLength = number_length(cardNumber);
        //printf("%i\n", cardLength);
        int checkType = 1;
        if (cardLength == 16)
        {
            int firstNum = cardNumber / 1e15;
            int firstPair = cardNumber / 1e14;
            if (firstNum == 4){
                printf("VISA\n");
                return 0;
            }
            else if (firstPair == 51 || firstPair == 52 || firstPair == 53 || firstPair == 54 || firstPair == 55){
                printf("MASTERCARD\n");
                return 0;
            }
            else
            {
                printf("INVALID\n");
                return 0;
            }
        }
        else if (cardLength == 15)
        {
            int firstPair = cardNumber / 1e13;
            if (firstPair == 34 || firstPair == 37)
            {
                printf("AMEX\n");
                return 0;
            }
            else
            {
                printf("INVALID\n");
                return 0;
            }
        }
        else if (cardLength == 13)
        {
            int firstNum = cardNumber / 1e12;
            if (firstNum == 4)
            {
                printf("VISA\n");
                return 0;
            }
            else
            {
                printf("INVALID\n");
                return 0;
            }
        }
        else {
            printf("INVALID\n");
        }
    }
    else {
        printf("INVALID\n");
        return 0;
    }
}

int calculate_sum1(long cardNumber)
{
    // let's make a function to find the first sum of the (even digits * 2) which we total before adding to the secondary sum
    //printf("\n%li\n", cardNumber);
    int sum1 = 0;
    do
    {
        cardNumber = cardNumber / 10; // take off the last digit
        int lasNum = (cardNumber % 10) * 2; // take the next digit on the right and multiply by 2
        if (lasNum > 9)
        { //if it's a two digit product, add both digits together to avoid using the product
            lasNum = (lasNum % 10) + (lasNum / 10);
        }
        cardNumber /= 10;            // take off the number we just used
        sum1 = sum1 + lasNum; //add our product/sum of digits to our total sum
    } while (cardNumber > 0);
    // printf("\n%i\n", sum1);
    return sum1;
}

int calculate_sum2(long cardNumber)
{ //let's make a similar funciton for the odd digits
    //printf("\n%li\n", cardNumber);
    int sum2 = 0;
    while (cardNumber > 0)
    {
        int lasNum = (cardNumber % 10); // take the last digit on the right
        cardNumber /= 10;             //take off the number we just used
        cardNumber /= 10;             // take off the next number so we're set up for the next loop
        sum2 += lasNum; //add the digit to our total sum
    }
    // printf("\n%i\n", sum2);
    return sum2;
}

int number_length(long cardNumber)
{
    int length = 0;
    for (long i = cardNumber; i > 0; i = i / 10)
    {
        length++;
    }
    return length;
}
