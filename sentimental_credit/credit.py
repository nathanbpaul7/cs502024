from cs50 import get_int

def main():
    cardNumber = get_int("Number: ") # prompt for credit card number
    # get first sum of first set of digits * 2 (with rule for oduble digit products)
    sum1 = calculate_sum1(cardNumber)
    # get next sum of second set of digits
    sum2 = calculate_sum2(cardNumber)

    # determine if card is valid
    total = sum1 + sum2

    if total % 10 == 0: # then its valid!  next, find out which card it is
        cardLength = len(str(cardNumber))
        firstPair = int(str(cardNumber)[:2])
        if cardLength == 16:
            if (cardNumber // 1e15 == 4):
                print("VISA")
            elif firstPair in range(51, 56):
                print("MASTERCARD")
            else:
                print("INVALID")
        elif (cardLength == 15):
            if firstPair == 34 or firstPair == 37:
                print("AMEX")
            else:
                print("INVALID")
        elif cardLength == 13:
            firstNum = cardNumber // 1e12
            if firstNum == 4:
                print("VISA")
            else:
                print("INVALID")
        else:
            print("INVALID")
    else:
        print("INVALID")


def calculate_sum1(cardNumber):
    sum1 = 0
    while cardNumber > 0:
        cardNumber //= 10 # take off last digit
        lasNum = (cardNumber % 10) * 2 # take the next digit on the right, multiply by 2
        if lasNum > 9:
            lasNum = (lasNum % 10) + (lasNum // 10)
        cardNumber //= 10
        sum1 = sum1 + lasNum
    return sum1

def calculate_sum2(cardNumber):
    sum2 = 0
    while cardNumber > 0:
        lasNum = (cardNumber % 10) # take the last digit on the right
        cardNumber //= 100             #  take off 2 numbers so we're set up for the next loop
        sum2 += lasNum              # add the digit to our total sum
    return sum2


main()

