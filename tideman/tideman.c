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
    for (int i = 0; i < candidate_count; i++)
    {
        if (!strcmp(candidates[i], name))
        {
            // Store index of candidate 'i' as voter's rank'th preference
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (ranks[i] ==  j)
            {
                // Increment preferences[current candidate in ranks array][next candidate ... last candidate in ranks array] by 1
                for (int k = i + 1; k < candidate_count; k++)
                {
                    if (!preferences[j][ranks[k]])
                    {
                        preferences[j][ranks[k]] = 1;
                    }
                    else
                    {
                        preferences[j][ranks[k]]++;
                    }
                }
                break;
            }
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Check if there are any voters who prefer candidate i to j
            if (preferences[i][j])
            {
                // Check if there are any voters who prefer candidate j to i, if not, add pair of candidates to pairs
                if (preferences[j][i])
                {
                    // Add pair of candidates to pairs unless there's a tie, i.e. preferences[i][j] == preferences[j][i]
                    if (preferences[i][j] > preferences[j][i])
                    {
                        pairs[pair_count].winner = i;
                        pairs[pair_count].loser = j;
                        pair_count++;
                    }
                    else if (preferences[i][j] < preferences[j][i])
                    {
                        pairs[pair_count].winner = j;
                        pairs[pair_count].loser = i;
                        pair_count++;
                    }
                }
                else
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                    pair_count++;
                }
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            // Swap i'th and (i + 1)'th elements of pairs array if more voters prefer the latter
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[j].winner][pairs[j].loser])
            {
                pair temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // Represents existence of a candidate with no arrow pointing at them
        bool flag;

        // Represents index of candidate with no arrow pointing at them
        int n;

        for (int j = 0; j < candidate_count; j++)
        {
            flag = true;
            n = j;

            for (int k = 0; k < candidate_count; k++)
            {
                // Move on to the next candidate if there's an arrow pointing at the current candidate
                if (locked[k][j])
                {
                    flag = false;
                    break;
                }
            }

            // Stop searching if a candidate with no arrow pointing at them is found
            if (flag)
            {
                break;
            }
        }

        // If there already exists a candidate with no arrow pointing at them and that candidate is not the loser of the current pair, lock the pair
        if (flag)
        {
            if (pairs[i].loser != n)
            {
                locked[pairs[i].winner][pairs[i].loser] = true;
            }
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    // Represents existence of a candidate with no arrow pointing at them
    bool flag;

    for (int i = 0; i < candidate_count; i++)
    {
        flag = true;

        for (int j = 0; j < candidate_count; j++)
        {
            // Move on to the next candidate if there's an arrow pointing at the current candidate
            if (locked[j][i])
            {
                flag = false;
                break;
            }
        }

        // If a candidate with no arrow pointing at them is found, print their name and exit the loop
        if (flag)
        {
            printf("%s\n", candidates[i]);
            break;
        }
    }
}