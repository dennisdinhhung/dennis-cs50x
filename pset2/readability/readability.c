#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int coleman_lieu(string input){ // applying the coleman_lieu to output the index
    float L, S;
    int count_word=0, count_letter=0, count_sent=0;
    char word;
    string letter;
    int index;

    //running thru the letters of the input 
    for (int i = 0; input[i] != '\0'; i++){
        
        // tolower the letters
        if (isupper(input[i])){
            input[i] = tolower(input[i]);
        }

        // words counting
        if (input[i] == ' '){
            count_word ++;
        }

        //letter counting
        if (input[i] >= 'a' && input[i] <= 'z'){
            count_letter ++;
        }

        //sentence counting
        if (input[i] == '!' || input[i] == '.' || input[i] == '?'){
            count_sent++;
        }
    }

    count_word = count_word + 1; //counting spaces would work except for the last word, so by adding 1 more, we would have the right number

    L = ((float)count_letter / (float)count_word) * 100;
    S = ((float)count_sent / (float)count_word) * 100;
    index = 0.0588*L - 0.296*S -15.8 + 0.5; // to add an extra 0.5 to round up the index num;
    return index;
};

int main(){
    string text = get_string("Text: ");

    int grade = coleman_lieu(text);

    if (grade >= 16){
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else{
        printf("Grade %d\n", grade);
    }
    
}