from cs50 import get_string


def get_input(prompt):
    # Function to prompt user for card number

    while True:
        card_number = get_string(prompt)
        if card_number.isdecimal():
            return card_number


def validate_card_number(number):
    # Function to validate card number entered by user

    sum = 0

    # Determine sum of alternate digits starting from last digit
    for i in range(len(number) - 1, -1, -2):
        sum += int(number[i])

    # Determine sum of double of alternate digits starting from penultimate digit
    for i in range(len(number) - 2, -1, -2):
        double = int(number[i]) * 2

        # Add 2 digits of double if it has more than 1 digit
        if len(str(double)) == 2:
            double = int(str(double)[0]) + int(str(double)[1])

        sum += double

    # Perform further checks if sum is divisible by 10, else return false
    if not sum % 10:
        length = len(number)

        if length == 13:
            return "VISA" if number[0:1] == "4" else "INVALID"
        elif length == 15:
            return "AMEX" if number[0:2] == "34" or number[0:2] == "37" else "INVALID"
        elif length == 16:
            n = int(number[0:2])
            if n >= 51 and n <= 55:
                return "MASTERCARD"
            elif n >= 40 and n <= 49:
                return "VISA"
            else:
                return "INVALID"
        else:
            return "INVALID"

    else:
        return "INVALID"


def main():
    card_number = get_input("Number: ")
    print(validate_card_number(card_number))


if __name__ == "__main__":
    # Call main function
    main()