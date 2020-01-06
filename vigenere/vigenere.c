#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int shift(char c);

int main(int argc, string argv[])
{
    // Returns if the number of command-line arguments is not 2
    if (argc != 2)
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        // Returns when a non-alphabetic character is found in the keyword
        if (!isalpha(argv[1][i]))
        {
            printf("Usage: ./vigenere keyword\n");
            return 1;
        }
    }
    // Prompts the user for a plaintext
    string plain_text = get_string("plaintext:  ");
    string keyword = argv[1];
    // Using index to access each character of the keyword
    int index = -1, limit = strlen(keyword) - 1;
    printf("ciphertext: ");
    for (int i = 0, length = strlen(plain_text); i < length; i++)
    {
        int character = plain_text[i];
        int case_constant;
        if (isupper(character))
        {
            case_constant = 'A';
        }
        else if (islower(character))
        {
            case_constant = 'a';
        }
        else
        {
            printf("%c", character);
            continue;
        }
        // Increments index by 1 but makes it 0 if it goes out of bound
        // thereby reusing the letters in the keyword cyclically
        index++;
        index = index > limit ? 0 : index;
        int key = shift(keyword[index]);
        // Applies the algorithm (i'th character of p + j'th character of k) % 26
        // but involves a case constant additionally since
        // alphabets are not numbered from 0 to 25 in ASCII
        int cipher = (character - case_constant + key) % 26 + case_constant;
        printf("%c", cipher);
    }
    printf("\n");
    return 0;
}

// Converts a character into the shift value
int shift(char c)
{
    // Using toupper function to ensure case insensitivity of the keyword
    int character = toupper(c) - 'A';
    return character % 26;
}
