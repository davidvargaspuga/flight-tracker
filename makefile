game: minesweep.o main.o load.o
	gcc -o minesweep minesweep.o main.o load.o

minesweep.o: minesweep.c minesweep.h cell.h
	gcc -c minesweep.c

main.o: main.c load.h cell.h minesweep.h
	gcc -c main.c

load.o: load.c load.h minesweep.h cell.h
	gcc -c load.c

clean: 
	rm minesweep minesweep.o main.o load.o
