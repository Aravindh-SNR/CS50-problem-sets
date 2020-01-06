#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Returns if the number of command-line arguments is not 2
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        // Returns when a non-numeric character is found in the key
        if (argv[1][i] < '0' || argv[1][i] > '9')
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    // Prompts the user for a plaintext
    string plain_text = get_string("plaintext:  ");
    int length = strlen(plain_text);
    // Converts the key from string to int
    int key = atoi(argv[1]);
    printf("ciphertext: ");
    for (int i = 0; i < length; i++)
    {
        int character = plain_text[i];
        int case_constant;
        if (character >= 'A' && character <= 'Z')
        {
            case_constant = 'A';
        }
        else if (character >= 'a' && character <= 'z')
        {
            case_constant = 'a';
        }
        else
        {
            printf("%c", character);
            continue;
        }
        // Applies the algorithm (i'th character of p + key) % 26
        // but involves a case constant additionally since
        // alphabets are not numbered from 0 to 25 in ASCII
        int cipher = (character - case_constant + key) % 26 + case_constant;
        printf("%c", cipher);
    }
    printf("\n");
    return 0;
}
