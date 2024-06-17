from cs50 import get_int

while True:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        break
for i in range(1, height + 1):
    spaceCount = height - i
    for spaces in range(spaceCount):
        print(" ", end="")
    for hashes in range(i):
        print("#", end="")
    print("  ", end="")
    for hashes in range(i):
        print("#", end="")
    print("")




