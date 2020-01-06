// Validates credit card numbers

#include <cs50.h>
#include <stdio.h>

long get_input(void);
string validate_number(long);

int main(void)
{
    long number = get_input();
    printf("%s\n", validate_number(number));
}

// Prompts the user for a positive card number
long get_input(void)
{
    long number;
    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);
    return number;
}

// Validates the card number
string validate_number(long number)
{
    int digits = 0, sum = 0;
    int temp;
    while (number > 0)
    {
        // Takes a digit or its double based on its position
        int digit = (digits == 0 || digits % 2 == 0) ? number % 10 : number % 10 * 2;
        // Adds the two digits if the result > 9, i.e. 12 would be 1 + 2 = 3
        if (digit > 9)
        {
            digit = 1 + digit % 10;
        }
        sum += digit;
        // Calculates the total number of digits in the card number
        digits++;
        // Stores the first 2 digits of the card number for later validation
        if (number > 9 && number < 100)
        {
            temp = number;
        }
        // Removes the last digit of the card number before the next iteration
        number /= 10;
    }
    if (sum % 10 == 0)
    {
        switch (digits)
        {
            case 13:
                return temp >= 40 && temp <= 49 ? "VISA" : "INVALID";
            case 15:
                return temp == 34 || temp == 37 ? "AMEX" : "INVALID";
            case 16:
                if (temp >= 51 && temp <= 55)
                {
                    return "MASTERCARD";
                }
                else if (temp >= 40 && temp <= 49)
                {
                    return "VISA";
                }
                else
                {
                    return "INVALID";
                }
            default:
                return "INVALID";
        }
    }
    else
    {
        return "INVALID";
    }
}
