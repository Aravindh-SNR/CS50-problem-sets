from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""

    # Split strings a and b into lines and store them as lists
    a_lines = a.splitlines()
    b_lines = b.splitlines()

    # Return a set of lines in both a and b
    return {line for line in a_lines if line in b_lines}


def sentences(a, b):
    """Return sentences in both a and b"""

    # Split strings a and b into English sentences and store them as lists
    a_sentences = sent_tokenize(a, language='english')
    b_sentences = sent_tokenize(b, language='english')

    # Return a set of sentences in both a and b
    return {sentence for sentence in a_sentences if sentence in b_sentences}


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    results = set()
    length = len(a) - n + 1

    # Iterate over string 'a' until substring of length n can be formed
    for i in range(0, length):
        # Form substring of length n
        substring = a[i:i+n]

        # Add substring to results if string 'b' also has it
        if substring in b:
            results.add(substring)

    return results