from cs50 import get_string
from cs50 import get_int

def main():
    # get input number
    number = get_int("Number: ")

    #convert number into a string for len() usage
    str_number = str(number)

    digit_count = [13,15,16]
    
    # check if the length of input number is valid, if not request the number again
    while len(str_number) not in digit_count:
        print("INVALID")
        return 1
    
    digit = []
    temp_number =  number

    # loop through the number and append the digits into digit[]
    for i in range(len(str_number)):
        temp = temp_number % 10
        digit.append(int(temp))
        temp_number = temp_number / 10

    # loop and add up the sum
    sum = 0
    for i in range(len(digit)):
        if i % 2 == 1:
            doubled = digit[i] * 2
            if doubled > 9:
                doubled = int((doubled/10)%10) + doubled%10
            sum += doubled
        else:
            sum += digit[i]

    # sum checking and printing the correct credit card
    if sum % 10 == 0:
        if len(digit) == 15:
            if digit[len(digit)-1] == 3 and (digit[len(digit)-2] == 4 or 7):
                print("AMEX")
            else: 
                print("INVALID")
        elif len(digit) == 16:
            if digit[len(digit)-1]*10 + digit[len(digit)-2] in range(51,56,1):
                print("MASTERCARD")
            elif digit[len(digit)-1] == 4:
                print("VISA")
            else:
                print("INVALID")
        elif digit[len(digit)-1] == 4 and len(digit) == 13:
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")
                

main()    