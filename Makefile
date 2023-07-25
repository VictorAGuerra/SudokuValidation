sudoku: sudoku.o
	cc sudoku.o -o sudoku
	touch sudoku.out
	rm *.o
	
sudoku.o: sudoku.c
	cc -c sudoku.c
