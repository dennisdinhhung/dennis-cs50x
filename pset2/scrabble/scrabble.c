#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main()
{
    // Get input words from both players

    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);
    
    // TODO: Print the winner
    if (score1 > score2){
        printf("Player 1 wins!\n");
    }
    else if (score1<score2)
    {
        printf("Player 2 wins!\n");
    }
    else if (score1==score2){
        printf("Tie!\n");
    }
    
}

int compute_score(string word)
{
// TODO: Compute and return score for string
    
    int score=0;
    char letter;

    //running thru the letters of the word one by one
    for (int i=0; word[i] != '\0'; i++){ //if word[i] == null then stop
        
        letter = word[i];

        //Lowercase the letters if needed

        if (letter >= 'A' && letter <= 'Z'){
            letter = letter + 32;
        }
        
        // Letter check with ASCII chart
        for (int i2=0; i2 < 26; i2++){
            
            int letter_check = i2 + 'a';
            
            if (letter == letter_check){
                score += POINTS[i2]; //Adding up the score
            }
        }
    }

    return score;
}