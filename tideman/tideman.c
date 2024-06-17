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
} pair;

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
bool cycleTest(int w, int l);
void print_winner(void);

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
    // check if name is valid, if so, update ranks to reflect rank of candidate
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++){
        for (int j = i + 1; j < candidate_count; j++){
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO: lets iterate through candidates to see if they have enough votes against an opposing candidate,
    // thereby creating a preferrred pair to add to our pair count
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int highest_win = 0;
    int n = 0;
    pair strongest_pair;
    pair pairsCopy[pair_count];
// find strongest victory by iterating through pairs and prefrences to extract highest victory margin
    for (int i = 0; i < pair_count; i++){
        if (preferences[pairs[i].winner][pairs[i].loser] > highest_win)
        {
            highest_win = preferences[pairs[i].winner][pairs[i].loser];
        }
    }
// then find any pair that matches strongest victory from array,
// move into ordered place in copied array and then decrease strongest victory and search for that. loop.
    while (n < pair_count){
        for (int k = highest_win; k >= 0; k--){
            for (int i = 0; i < pair_count; i++){
                if (preferences[pairs[i].winner][pairs[i].loser] == k){
                    pairsCopy[n] = pairs[i];
                    n++;
                }
            }
        }
    }
    for (int i = 0; i < pair_count; i++){
        pairs[i] = pairsCopy[i];
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int w = 0;
    int l = 0;
    // let's iterate through array of sorted pairs, deciding if they can lock in as we go!
    for (int i = 0; i < pair_count; i++){
        w = pairs[i].winner;
        l = pairs[i].loser;
        if (!cycleTest(w, l)){ // if cycleTest is false (no cycle exists), pair is safe to lock in
            locked[w][l] = true;
        }
        else {
            continue;
        }

    }
    return;
}
bool cycleTest(int w, int l)
{
    //base case, if the edge points to our origin, the winner input will show up as a loser input
    if (l == w)
        return true;
    // iterate through possible edges with their origin from the loser
    for (int i = 0; i < candidate_count; i++){
        if (locked[l][i]){
            if (cycleTest(w, i))
                return true;
        } // if the edge exists, see if the candidate it points to ever cycles back to OG winner -- help on recursion from great minds at  https://gist.github.com/nicknapoli82/6c5a1706489e70342e9a0a635ae738c9

    }
    return false;
}
// Print the winner of the election

void print_winner(void)
{
    //selection sort through locked edges, seeing which candidate never shows up as a loser
    int winner = 0;

    for (int i = 0; i < candidate_count; i++){
        for (int j = 0; j < candidate_count; j++) {
            if (locked[j][i]) // if the candidate loses in any locked edge, then go to check next candidate
                break;
            else
            winner = i;
        }
    }
    printf ("%s\n", candidates[winner]);
    return;
}
