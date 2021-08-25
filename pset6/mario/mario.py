from cs50 import get_int

def main():
    height = get_int("Height: ")

    while height < 1 or height > 8:
        height = get_int("Height: ")

    
    for i in range(height):
        # spaces
        for a in range(height-i-1):
            print(" ", end="")

        # left side pyramid
        for b in range(i+1):
            print("#", end="")

        #space between the pyramids
        print("  ", end="")

        # right side pyramid
        for j in range(i+1):
            print("#", end="")
        
        print("") #new line

main()