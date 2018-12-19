addi 3 16 3 # jmp 18
seti 1 8 4 # r[4] = 1
seti 1 4 5 # r[5] = 1
mulr 4 5 1 # r[1] = r[4] * r[5] == 1
eqrr 1 2 1 # r[1] = r[1] == r[2] == 0
addr 1 3 3 # jump + r[1]
addi 3 1 3 # jump 9
addr 4 0 0 # r[0] = r[4] + r[0]
addi 5 1 5 # r[5] += 1
gtrr 5 2 1 # r[1] = r[5] > r[2]
addr 3 1 3 # jump + r[1]
seti 2 1 3 # jump 4
addi 4 1 4 # r[4] = r[4] + 1
gtrr 4 2 1 # r[1] = r[4] > r[2]
addr 1 3 3 # jump + r[1]
seti 1 3 3 # jmp 3
mulr 3 3 3 # END
addi 2 2 2 # r[2] = r[2] + 2
mulr 2 2 2 # r[2] = r[2] * r[2]
mulr 3 2 2 # r[2] = r[3] * r[2]
muli 2 11 2 # r[2] = r[2] * 11
addi 1 3 1 # r[1] = r[1] + 3
mulr 1 3 1 # r[1] = r[1] * r[3]
addi 1 17 1 # r[1] = r[1] + 17
addr 2 1 2 # r[2] = r[2] + r[1]
addr 3 0 3 # jump + r[0]
seti 0 3 3 # jump 2
setr 3 0 1 # r[1] = r[3]
mulr 1 3 1 # r[1] = r[1] * 28
addr 3 1 1 # r[1] = r[1] + 29
mulr 3 1 1 # r[1] = r[1] * 30
muli 1 14 1 # r[1] = r[1] * 14
mulr 1 3 1 # r[1] = r[1] * 32
addr 2 1 2 # r[2] = r[2] + r[1]
seti 0 8 0 # r[0] = 0
seti 0 9 3 # jump 2
