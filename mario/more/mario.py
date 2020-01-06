from cs50 import get_int


def get_positive_int(prompt):
    # Function to get positive integer not greater than 8
    while True:
        height = get_int(prompt)
        if height > 0 and height <= 8:
            return height


def main():
    # Prompt user for pyramid height
    height = get_positive_int("Height: ")

    # Generate pyramid
    for line in range(1, height + 1):

        # Print (height - line) number of leading white space characters
        print(" " * (height - line), end="")

        # Print line number of hashes (#)
        print("#" * line, end="")

        # Print 2 white space characters
        print("  ", end="")

        # Print line number of hashes (#) again
        print("#" * line)


if __name__ == "__main__":
    main()