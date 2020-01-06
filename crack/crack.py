from sys import argv
from crypt import crypt


def main():
    # Ensure correct usage
    if not len(argv) == 2:
        exit("Usage: python crack.py hash")

    # Store hash and salt
    hash = argv[1]
    salt = argv[1][0:2]

    # Start brute force comparison with 'a'
    password = 'a'

    while len(password) <= 5:
        # Compare hash generated with hash entered by user
        if crypt(password, salt) == hash:
            print(password)
            break

        """Loop through characters of password starting from the last down to the first and
        change one character at a time, thereby trying out all possible combinations of alphabets"""

        length = len(password)
        for i in range(length - 1, -1, -1):
            if password[i] == 'z':

                # Change character to uppercase on reaching 'z' and break out of for loop to keep continuing till 'Z'
                password = password[:i] + 'A' + password[i+1:]
                break

            elif password[i] == 'Z':
                if not i:

                    """If the first character of password reaches 'Z', one more character is added to password
                    and all the characters start with 'a' again, e.g. Z becomes aa, ZZ becomes aaa, etc."""

                    password = 'a' * (length + 1)

                else:

                    """Start again from 'a' on reaching 'Z' but the previous character is also incremented by 1 now
                    // e.g. aZ becomes ba, bZ becomes ca, etc."""

                    password = password[:i] + 'a' + password[i+1:]
            else:
                # Increment character by 1 and break out of for loop to keep continuing until 'z' or 'Z' is reached
                password = password[:i] + chr(ord(password[i]) + 1) + password[i+1:]
                break


if __name__ == "__main__":
    # Call main function
    main()