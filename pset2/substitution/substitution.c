#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string key[]){
    int x;
    char letter;

    //key checking with argc
    if (argc != 2){
        printf("Usage: ./substitution key\n");
        return 1;
    }

    //Check 26 char length
    if (strlen(key[1]) != 26){
        printf("Key must contain 26 characters.");
        return 1;
    }
    
    //char checking for anything other than letters
    for (int i = 0; key[1][i] != '\0'; i++){
        if (key[1][i] < 'A' || (key[1][i] > 'Z' && key[1][i] < 'a') || key[1][i] > 'z'){
            printf("The key should not use anything other than letters\n");
            return 1;
        }
        
        char key_char1 , key_char2;
        //char checking for repeats of letters
        for (int i2 = i+1; key[1][i2] != '\0'; i2++){
            key_char2 = toupper(key[1][i2]);
            key_char1 = toupper(key[1][i]);
            if (key_char2 == key_char1){
                printf("The key should not have repeats of characters.\n");
                return 1;
            }
        }
    }


    
    
    //asking user for input
    string input = get_string("plaintext: ");

    //init the ciphertext ouput
    printf("ciphertext: ");

    //run over every letter of the input
    for (int i = 0; input[i] != '\0'; i++){

        //conversion
        if (input[i] >= 'a' && input[i] <= 'z'){ //lowercase convert
            x = input[i] - 'a';
            letter = key[1][x];

            if (isupper(letter)){
                letter = tolower(letter);
            }
            printf("%c", letter);
        } 
        else if (input[i] >= 'A' && input[i] <= 'Z'){ //uppercase convert
            x = input[i] - 'A';
            letter = key[1][x];

            if (islower(letter)){
                letter = toupper(letter);
            }
            printf("%c", letter);
        }
        else{
            printf("%c", input[i]); //print anything that is other than letters
        }
    }
    printf("\n");
    return 0;
}