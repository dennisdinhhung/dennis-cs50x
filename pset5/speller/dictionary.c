// Implements a dictionary's functionality
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = (26+1)*2; // 26 for the letters of the alphabet, +1 for

// Hash table
node *table[N];

int count_word=0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //? Instructions
        //* return true if word is found
        //* return false if not
        //*  Case insensitive
    // DONE
        //* Obtain hash value

    int hash_num = hash(word);
        //* Access the linked list at said value

    node *cursor;
    cursor = table[hash_num];

    if (cursor == NULL){
        return false;
    }

    while(strcasecmp(cursor->word,word) != 0){
        //* Traverese the list, look for the word (use strcasecmp())
        cursor = cursor->next;

        if (cursor == NULL){
            return false;
        }
    }

    if (strcasecmp(cursor->word,word)==0){
        return true;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // DONE
    //input: word (maybe apostrophes)
    //output: index number (between 0 and N-1)

    // the hash num does not need to be in increase order for AA, AB,... It needs to be consistent everytime
    int index;
    char letter1, letter2;

    letter1 = tolower(word[0]);
    letter2 = tolower(word[1]);

    index = (letter1+letter2) % N;

    return index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    //Open file
    FILE *input_dict = fopen(dictionary, "r");
    //** Check if input is NULL
    if(input_dict == NULL){
        printf("Could not open file!\n");
        return false; //cause the fuction returns a bool
    }
    char holder[LENGTH+1];
    //Read string from file repeat until you get EOF
    while(fscanf(input_dict, "%s", holder) != EOF){
        //Make a new node
        node *n = malloc(sizeof(node));

            //** Check if malloc returns NULL
        if (n==NULL){
            printf("Cannot malloc n!\n");
            return false;
        }

            //** Copy from holder to n->word (using strcpy)
        strcpy(n->word, holder);
        n->next = NULL;

        //Hash the word (use the premade hash function)
        int hash_num = hash(n->word);
        size();

        //Insert node into hash table
            //* Point the word new_node to the next word in the table
        n->next = table[hash_num];

            //* Point table's head to the new_node
        table[hash_num] = n;
    }

    fclose(input_dict);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    //DONE
    count_word++;
    return (count_word - 1); //since the function to print count_word also calls the function again, return needs to minus the count_word by 1
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *cursor;
    
    //Setting up cursor

    for (int i=0; i<N; i++){
        //Set up
        cursor = table[i];

        while(cursor != NULL){
            node *temp;
            temp = cursor;

            //Move cursor to next
            cursor = cursor->next;

            free(temp);
        }
    }
    free(cursor);
    return true;
}
