// Implements a dictionary's functionality

#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 10000

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];
bool loaded = false;
int word_count = 0;

// Hashes word to a number between 0 and 25, inclusive
unsigned int hashWord(const char *word)
{
    //hash function from https://stackoverflow.com/questions/14409466/simple-hash-functions?rq=1
    unsigned int counter;
    unsigned int hashAddress = 5381;
    for (counter = 0; word[counter]; counter++)
    {
        hashAddress = ((hashAddress << 5) + hashAddress) + word[counter];
    }

    return hashAddress % N;
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
        // increment dictionary size
        word_count++;

        // allocate memory for new word
        node* newWord = malloc(sizeof(node));

        // put word in the new node
        strcpy(newWord->word, word);

        // find what index of the array the word should go in
        int index = hashWord(word);

        // if hashtable is empty at index, insert
        if (hashtable[index] == NULL)
        {
            hashtable[index] = newWord;
            newWord->next = NULL;
        }

        // if hashtable is not empty at index, append
        else
        {
            newWord->next = hashtable[index];
            hashtable[index] = newWord;
        }
    }

    // Close dictionary
    fclose(file);
    loaded = true;
    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (loaded)
    {
        return word_count;
    }
    else
    {
        return 0;
    }
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char word_lower[strlen(word) + 1];
    strcpy(word_lower, word);

    for (int i = 0; i < strlen(word); i++)
    {
        word_lower[i] = tolower(word_lower[i]);
    }

    node *loop_node = hashtable[hashWord(word_lower)];
    while (loop_node != NULL)
    {
        if (strcmp(loop_node->word, word_lower) == 0)
        {
            return true;
        }
        else
        {
            loop_node = loop_node->next;
        }
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node* cursor = hashtable[i];
        while (cursor != NULL)
        {
            // maintain connection to linked list using temp
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    loaded = false;
    return true;
}
