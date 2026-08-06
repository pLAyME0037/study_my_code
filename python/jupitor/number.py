#!/usr/bin/env python3

num = float(input("number: "))

# if (num == 0):
#     print("Zero")
# elif (num >= 1e-10000):
#     print("Positive")
# else:
#     print("Negative")

if (num == 0):
    print("Zero")
elif (num - 1 <= -2):
    print("Negative")
elif (num - 1 >= 0):
    print("Positive")
else:
    print("Non Number")

