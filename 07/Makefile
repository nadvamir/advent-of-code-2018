CC=g++-6
PROGRAM_1=sleigh
PROGRAM_2=sleigh2

all: p1 p2

p1:
	$(CC) --std=c++17 -O2 $(PROGRAM_1).cpp -o $(PROGRAM_1).exe
	cat data.in | ./$(PROGRAM_1).exe

p2:
	$(CC) --std=c++17 -O2 $(PROGRAM_2).cpp -o $(PROGRAM_2).exe
	cat data.in | ./$(PROGRAM_2).exe
