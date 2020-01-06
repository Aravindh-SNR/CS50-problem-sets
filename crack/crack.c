// Password cracker

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>

int main(int argc, char *argv[])
{
    // Returns if the number of command-line arguments is not 2
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    // Stores the second command-line argument as hash
    char *hash = argv[1];
    
    // Stores the first two characters of the hash as salt
    char *salt = malloc(3 * sizeof(char));
    *salt = *hash;
    *(salt + 1) = *(hash + 1);
    
    // Starting brute force comparison with 'a'
    char *password = malloc(6 * sizeof(char));
    *password = 'a';
    
    while (strlen(password) <= 5)
    {
        // Generates a hash with the salt and current password using C's DES-based crypt function
        char *hash2 = crypt(password, salt);
        
        // Compares the hash generated with the hash entered by the user, prints the password and returns 0 if they match
        if (!strcmp(hash2, hash))
        {
            printf("%s\n", password);
            //Frees memory
            free(salt);
            free(password);
            return 0;
        }
        
        // Loops through the characters of the password starting from the last character down to the first
        // and changes one character at a time, thereby trying out all possible combinations of alphabets
        int length = strlen(password);
        for (int i = length - 1; i >= 0; i--)
        {
            if (*(password + i) == 'z')
            {
                // Changes the character to uppercase on reaching 'z' and breaks out of the for loop to keep continuing till 'Z'
                *(password + i) = 'A';
                break;
            }
            else if (*(password + i) == 'Z')
            {
                if (i == 0)
                {
                    // If the first character of the password reaches 'Z', one more character is added to the password
                    // and all the characters start with 'a' again, e.g. Z becomes aa, ZZ becomes aaa, etc.
                    for (int j = 0; j <= length; j++)
                    {
                        *(password + j) = 'a';
                    }
                }
                else
                {
                    // Starts again from 'a' on reaching 'Z' but the previous character is also incremented by 1 now
                    // e.g. aZ becomes ba, bZ becomes ca, etc.
                    *(password + i) = 'a';
                }
            }
            else
            {
                // Increments the character by 1 and breaks out of the for loop to keep continuing until 'z' or 'Z' is reached
                *(password + i) += 1;
                break;
            }
        }
    }
    //Frees memory
    free(salt);
    free(password);
    // Returns 1 if the password is not cracked
    return 1;
}

//Passwords cracked using this program:

// brian:51.xJagtPnb6s      => TF
// bjbrown:50GApilQSG3E2    => UPenn
// emc:502sDZxA/ybHs        => puppy
// greg:50C6B0oz0HWzo       => FTW
// jana:50WUNAFdX/yjA       => Yale
// lloyd:50n0AAUD.pL8g      => lloyd
// malan:50CcfIk1QrPr6      => maybe
// natmelo:50JIIyhDORqMU    => nope
// rob:51v3Nh6ZWGHOQ        => ROFL
// veronica:61v1CDwwP95bY   => hola
// walker:508ny6Rw0aRio     => sean
// zamyla:50cI2vYkF0YU2     => LOL
