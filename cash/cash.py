from cs50 import get_float


def get_positive_change_owed():
    # Function to prompt user for non-negative change owed

    while True:
        change_owed = get_float("Change owed: ")
        if change_owed > 0:
            return change_owed


def get_coins(change_owed):
    # Function to calculate minimum number of coins that can be made

    # Convert dollars into cents
    cents = round(change_owed * 100)

    # Calculate number of quarters
    coins = cents // 25
    cents %= 25

    # Calculate number of dimes
    coins += cents // 10
    cents %= 10

    # Calculate number of nickels
    coins += cents // 5
    cents %= 5

    # Calculate number of pennies
    coins += cents

    return coins


def main():
    change_owed = get_positive_change_owed()
    print(get_coins(change_owed))


if __name__ == "__main__":
    # Call main function
    main()