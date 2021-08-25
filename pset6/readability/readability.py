from cs50 import get_string
import string

def coleman(input):
    count_letter = count_sent = 0
    count_word = 1

    for letter in input: # loop thru input with letter as a char
        
        if letter == ' ':
            count_word += 1
        
        if letter in list(string.ascii_letters):
            count_letter += 1
        
        punc = ['!','.','?']
        if letter in punc:
            count_sent += 1

    L = (count_letter/count_word) * 100
    S = (count_sent/count_word) * 100

    index = 0.0588*L - 0.296*S - 15.8 + 0.5

    return index

def main():
    text = get_string("Text: ")
    grade = int(coleman(text))

    if grade >= 16:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print("Grade " + str(grade))


main()