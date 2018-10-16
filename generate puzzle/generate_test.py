import sys
import random

# print(sys.argv)

size = int(sys.argv[1])
total_moves = int(sys.argv[2])

table = [[i % size ** 2 for i in range(j * size + 1, j * size + 1 + size)] for j in range(size)]
blank = (size - 1, size - 1)

op = [(1, 0), (-1, 0), (0, 1), (0, -1)]
off = tuple()
result = []
for n in range(total_moves):
	while True:
		next_op = random.choice(op)
		if next_op != off:
			break
	off = (next_op[0] * -1, next_op[1] * -1)
	target = ((blank[0] + next_op[0]) % size, (blank[1] + next_op[1]) % size)
	result.append(tuple(reversed(blank)))
	table[blank[0]][blank[1]] = table[target[0]][target[1]]
	table[target[0]][target[1]] = 0
	blank = target
result.reverse()
print(table)
print(result)
with open('test.txt', 'w') as f:
	f.write("{}\n".format(size))
	for row in table:
		line = ""
		for column in row:
			line += "{},".format(column)
		f.write("{}\n".format(line[:-1]))

	

