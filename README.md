# SudokuValidation

## Description

The purpose of the code is to verify, with the use of multithreads, if the inserted Sudoku parameters and matrix are valid, no matter the size of the game.
Meaning it will check if there are any inconsistensies with the given data, in comparison to the Sudoku game model.

## How to use

The code is used with an additional **Makefile** that compiles the code and returns an executable file called "sudoku",
and an additional input file. the input file has no specified name, so the user will specify where to take the input data from.
When using the program, first enter the directory with the program files via terminal and run the "make" command, that will create de "sudoku" file.
Then, run the "./sudoku inputexample.txt" command. (inputexample will be the name of the input file name).
The output will be printed through a "sudoku.out" file that will be created by the program.

* The input file must exist previously. If the name given as an argument in command line cannot be found, it will be notified in the .out file.
* There must be only two arguments in command line when running the "./sudoku (file)" command. Any format aside will be notified in the .out file.
* The game matrix must have at least the standard sudoku game size (3 by 3 subgrid, 9 by 9 grid, 81 numbers).
  
## Returns

* The code will return "SUCCESS" if there are no errors in the sudoku game.
* If the program checks an error in the game, it will return "FAIL".
* If there were errors in the compliation / execution, it will print it in the .out file.

## Input file format

* (size of rows)x(size of columns)
* (row size of subgrid)x(column size of subgrid)
* (matrix)

## Aditional info

All the application is written in C language and utilizes threads for the sudoku validation. It was passed as an activity for "Software Infrastructure" class in Computer Science graduation at CESAR School.
