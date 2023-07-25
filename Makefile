sudoku: sudoku.o
	cc sudoku.o -o sudoku
	touch sudoku_vag.out
	rm *.o
	
sudoku.o: sudoku.c
	cc -c sudoku.c
