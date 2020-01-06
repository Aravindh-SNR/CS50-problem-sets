// Calculates the minimum number of coins with which a change can be made

#include <cs50.h>
#include <stdio.h>
#include <math.h>

float get_input(void);
int get_coins(float);

int main(void)
{
    float change_owed = get_input();
    printf("%i\n", get_coins(change_owed));
}

// Prompts the user for a non-negative change
float get_input(void)
{
    float change_owed;
    do
    {
        change_owed = get_float("Change owed: ");
    }
    while (change_owed < 0);
    return change_owed;
}

// Returns number of coins
int get_coins(float change)
{
    // Converts dollars into cents
    int cents = round(change * 100);
    
    // Calculates number of quarters
    int coins = cents / 25;
    cents %= 25;
    
    // Calculates number of dimes
    coins += cents / 10;
    cents %= 10;
    
    // Calculates number of nickels
    coins += cents / 5;
    cents %= 5;
    
    // Calculates number of pennies
    coins += cents;
    
    return coins;
}
