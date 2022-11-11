def convert_bin(x: int, n: int = 8) -> str:
    if x > 2 ** (n - 1) - 1 or x < -(2 ** (n - 1)):
        raise ValueError("Too big")
    if x < 0:
        x = 2**n + x
    return bin(x)[2:].zfill(n)


def bin_to_dec(x: str) -> int:
    if x[0] == "1":
        return int(x, 2) - 2 ** len(x)
    return int(x, 2)


def ieee754(l: list) -> float:
    """Convert a list of 64 bits to a float"""
    l = list(l)

    # Extract from the list
    s = l[0]
    e = l[1:12]
    m = l[12:] + ["0"] * (len(l) - 12)

    # Convert to int
    s = int(s)
    e = int("".join(e), 2)
    m = int("".join(m), 2)

    # Constant b
    b = 2**10 - 1

    # Base case
    if 0 < e - b < 2**11 - 1:
        return ((-1) ** s) * (1 + m * (2**-52)) * (2 ** (e - b))
    # Special cases
    elif e == 0:
        return (-1) ** s * (m * 2**-52) * (2**-1022)
    elif e == 2**11 - 1:
        if m == 0:
            return float("inf")
        else:
            return float("nan")
    else:
        raise ValueError(f"Invalid : {e=}")


def print_list_f(l: list, f: callable) -> None:
    for i in l:
        try:
            print(i, f(i), sep="\t")
        except Exception as e:
            print(i, e, sep="\t")


def main():
    l = [9, -18, -48, -128, 129]
    l2 = [
        "00111001",
        "11000010",
        "01100110",
        "10001101",
    ]
    print_list_f(l, convert_bin)
    print_list_f(l2, bin_to_dec)

    print(ieee754("0100000001010101010010000000000000000000000000000000000000000000"))


if __name__ == "__main__":
    main()
