#!/bin/env python3

guests = ["Mark", "Jerry", "Bob", 
          "David", "Chris", "Tina"]

print("There are too many guests invited to dinner!")

print("")

while len(guests) > 2:
    g = guests.pop()
    print(f"Goodbye {g}...")

print("")

print(f"{guests[0]} and {guests[1]} are coming to dinner.")
