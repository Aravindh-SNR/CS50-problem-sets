from cs50 import get_string
from sys import argv


def main():
    # Ensure correct usage
    if not len(argv) == 2:
        exit("Usage: python bleep.py dictionary")

    # Store filename and message to be censored
    file_name = argv[1]
    message = get_string("What message would you like to censor?\n").split()

    # Open file containing banned words and add them to a set
    words = set()
    file = open(file_name, "r")
    for line in file:
        words.add(line.strip())
    file.close()

    # Iterate over words in message and print them if they are not in the set, else print * for each character of the word
    for word in message:
        if word.lower() in words:
            print("*" * len(word), end=" ")
        else:
            print(word, end=" ")

    print()


if __name__ == "__main__":
    main()
