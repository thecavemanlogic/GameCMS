import random, sys

hands = ["ROCK", "PAPER", "SCISSORS"]

for i in range(300):
	print("Waiting for input...", file=sys.stderr)
	ready = input()
	print(ready, file=sys.stderr)
	# print(random.choice(hands))
	print("PAPER", flush=True)
	print("PAPER", file=sys.stderr)
	result = input()
	print(result, file=sys.stderr)
