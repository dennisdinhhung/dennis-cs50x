#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// Extra function
int loop_check(int loser);

// Main
int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO -> Done (have to update the ranks[i] to the ith's pref which is also the value of rank that was determined by the voter)
    for (int i=0; i<candidate_count; i++){
        if(strcmp(name, candidates[i]) == 0){
            ranks[rank] = i;//insert rank here
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO -> Done (no need to fix, this was correct from the start :) )
    for (int i=0; i<candidate_count; i++){
        for (int j = i+1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
        
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO -> DONE
    for (int i=0; i<candidate_count; i++){
        for (int j=i+1; j<candidate_count; j++){
            if (preferences[i][j]>preferences[j][i]){
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count +=1;
            }
            else if (preferences[i][j]<preferences[j][i]){
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count +=1;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO -> DONE

    // Making an extra int for place holder
    int temp_winner, temp_loser, temp_diff;
    int diff[pair_count];
    bool repeat = true;
    // create a diff array 
    for(int i = 0; i < pair_count; i++){
        
        diff[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
    }

    // Sorting with Bubble Sort, bigger on top, lower on bottom
    while(repeat==true){ // Have the loop repeat itself to check if there is any unsorted items.
        
        repeat = false;
        for(int i = 0; i < pair_count; i++){
            int j = i+1;
            if (diff[i] < diff[j]){
                temp_winner = pairs[i].winner;
                temp_loser = pairs[i].loser;

                pairs[i] = pairs[j];

                pairs[j].winner = temp_winner;
                pairs[j].loser = temp_loser;

                temp_diff = diff[i];
                diff[i] = diff[j];
                diff[j] = temp_diff;

                repeat = true;
            }
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    
    for (int i = 0; i < pair_count; i++){
        // lock in the first pair in the pairs array
        int x = pairs[i].winner;
        int y = pairs[i].loser;
        
        if (loop_check(y) != x){
            locked[x][y] = true;
        }
    }

    return;
}

int loop_check(loser){//recursion function
    for (int i = 0; i < candidate_count; i++){
        if (locked[loser][i]){
            return loop_check(i);
        }
    }
    return loser;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}

