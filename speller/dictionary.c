// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;

// Represents number of words in dictionary which will be incremented in load function
unsigned int count = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
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

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // Start traversing from root node
        node *n = root;

        // Iterate over characters of word
        for (int i = 0, length = strlen(word); i < length; i++)
        {
            // Determine unique index for each character
            int index = (word[i] == '\'') ? 26 : tolower(word[i]) - 'a';

            // Go down the path if there exists one at index, otherwise create new path
            if (n->children[index])
            {
                n = n->children[index];
            }
            else
            {
                node *child = malloc(sizeof(node));
                if (!child)
                {
                    return false;
                }
                child->is_word = false;
                for (int j = 0; j < N; j++)
                {
                    child->children[j] = NULL;
                }
                n->children[index] = child;
                n = child;
            }
        }

        // Mark is_word of last created node true signifying end of word
        n->is_word = true;
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
    // Start traversing from root node
    node *n = root;

    // Iterate over characters of word
    for (int i = 0, length = strlen(word); i < length; i++)
    {
        // Determine unique index for each character
        int index = (word[i] == '\'') ? 26 : tolower(word[i]) - 'a';

        // Go down the path if there exists one at index, otherwise return false
        if (n->children[index])
        {
            n = n->children[index];
        }
        else
        {
            return false;
        }
    }

    // Return the value of is_word of last traversed node, which signifies if the word is complete or not
    return n->is_word;
}

// Unloads dictionary from memory recursively
void recursive_unload(node *n)
{
    for (int i = 0; i < N; i++)
    {
        if (n->children[i])
        {
            recursive_unload(n->children[i]);
        }
    }
    free(n);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    recursive_unload(root);
    return true;
}