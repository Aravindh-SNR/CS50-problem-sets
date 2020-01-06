// Recreates Mario pyramids using hashes

#include <cs50.h>
#include <stdio.h>

int get_input(void);
void create_pyramid(int height);

int main(void)
{
    int height = get_input();
    create_pyramid(height);
}

// Prompts the user for a height between 1 and 8, inclusive
int get_input(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);
    return height;
}

// Prints n number of hashes
void print_hash(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("#");
    }
}

// Creates a pyramid of hashes of given height
void create_pyramid(int height)
{
    for (int line = 1; line <= height; line++)
    {
        // Prints (height - line) number of space characters
        for (int i = line; i < height; i++)
        {
            printf(" ");
        }
        print_hash(line);
        printf("  ");
        print_hash(line);
        printf("\n");
    }
}
