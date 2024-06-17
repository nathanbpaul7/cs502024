// Implements a dictionary's functionality - big props to stack Exchange and users who posted about their trie structures
// and gave me tips for how to cut down on runtime by calculating sizeOf the node and placing it in constant, also for code related to recursive function for memory freeing
// https://cs50.stackexchange.com/questions/25668/unload-function-in-speller-c-failing-miserably-despite-seemingly-correct-code

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"


// Trie Node Structure
typedef struct node
{
    bool wordExists;
    struct node *children[27];
} node;

// recursive function for freeing trie roadmap at end of program
bool free_trieRoadmap(node *trie_root);

// Declare Global Variables
node *root = NULL;
int nodeSize = sizeof(node);
unsigned int wordcount = 0;
unsigned int loaded = 0;


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *branch = root;
    for (int i = 0, l = strlen(word); i < l; i++)
    {
        int hash = 0;
        if (isalpha(word[i]))
        {
            hash = tolower(word[i]) - 'a';
            if (branch->children[hash] == NULL)
                return false;
            else
            {
                branch = branch->children[hash];
            }
        }
        else if (word[i] == '\'') // same as above but hash set to 26
        {
            hash = 26;
            if (branch->children[hash] == NULL)
                return false;
            else
            {
                branch = branch->children[hash];
            }
        }
        if (i == l - 1)
        {
            if (branch->wordExists)
                return true;
        }
    }
    return false;
}


// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");

    // Check to make sure it opened correctly
    if (source == NULL)
    {
        fclose(source);
        printf("Unable to load %s\n", dictionary);
        return false;
    }
    // intilize root of trie and branch
    root = calloc(1, nodeSize);
    if (root == NULL)
        return false;
    node *branch = root;
    // Read each word in the file
    for (int c = fgetc(source), hash = 0; c != EOF; c = fgetc(source))
    {
        // Add each word to the trie roadmap
        if (isalpha(c)) // if it's a letter in the alphabet, case insensitive
        {
            // handle any upper case problems, set hash code
            hash = tolower(c) - 'a';

            // adjust branch!
            if (branch->children[hash] == NULL)
            {
                branch->children[hash] = calloc(1, nodeSize);
                if (branch->children[hash] == NULL)
                    return false;
            }
            branch = branch->children[hash];

        }
        else if (c == '\'') // if it's an apostrophe, still map it!
        {
            hash = 26;

            if (branch->children[hash] == NULL)
            {
                branch->children[hash] = calloc(1, nodeSize);
                if (branch->children[hash] == NULL)
                    return false;
            }
            branch = branch->children[hash];
        }
        else if (c == '\n') // word is finished, increase word count, set boolean, set branch to root
        {
            branch->wordExists = true;
            wordcount++;
            branch = root;
        }
    }
    loaded = 1;
    // Close the dictionary file
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (loaded == 0)
        return 0;
    else
        return wordcount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    if (free_trieRoadmap(root))
        return true;
    else
        return false;
}

bool free_trieRoadmap(node *trieroot)
{
    for (int i = 0; i < 27; i++)
    {
        if(trieroot->children[i] != NULL)
        {
            free_trieRoadmap(trieroot->children[i]);
        }
    }
    free(trieroot);
    return true;
}
