// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Number of words in dictionary, which will be incremented in load function
unsigned int count = 0;

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node
        node *n = malloc(sizeof(node));
        strcpy(n->word, word);

        // Hash word to a number
        unsigned int hash_n = hash(word);

        // Insert word into hashtable
        if (!hashtable[hash_n])
        {
            n->next = NULL;
        }
        else
        {
            n->next = hashtable[hash_n];
        }
        hashtable[hash_n] = n;

        // Count number of words in dictionary
        count++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Hash word to a number
    unsigned int hash_n = hash(word);

    // Ensure case-insensitive comparison
    int length = strlen(word);
    char new_word[length + 1];
    for (int i = 0; i < length; i++)
    {
        new_word[i] = tolower(word[i]);
    }
    new_word[length] = '\0';

    // Iterate over linked list at index 'hash_n' of hashtable
    for (node *n = hashtable[hash_n]; n; n = n->next)
    {
        if (!strcmp(n->word, new_word))
        {
            return true;
        }
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Iterate over arrays of hashtable
    for (int i = 0; i < N; i++)
    {
        node *n = hashtable[i];

        // Iterate over linked list at each index of the array
        while (n)
        {
            node *ptr = n;
            n = n->next;

            // Free memory allocated for linked list node
            free(ptr);
        }
    }

    return true;
}