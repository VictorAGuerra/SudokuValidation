#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int file_errors = 0;
int sudoku_errors = 0;

int A;
int B;
int N;

int *ptr;

typedef struct
{
  int id;
} datastruct;

void grid_size_validation(unsigned long input_count, int N);

void subgrid_size_validation(int A, int B, int N);

void *row_validation(void *data);

void *column_validation(void *data);

void *subgrid_validation();

int main (int argc, char *argv[])
{
  //verify command line
  if (argc != 2)
  {
    FILE *output = fopen("sudoku.out", "w");
    fprintf(output, "Invalid number of parameters");
    fclose(output);
    exit(EXIT_FAILURE);
  }
  
  char namefile[100];
  strcpy(namefile, argv[1]);
  
  //verify existence of input file
  if (access(namefile, F_OK) == -1)
  {
    FILE *output = fopen("sudoku.out", "w");
    fprintf(output, "Error to read file");
    fclose(output);
    exit(EXIT_FAILURE);
  }
   
  FILE *fptr = fopen(namefile, "r");
  
  //acquiring A, B, and N
  char N1[15], N2[15], A1[15], B1[15];
  int i;
  
  fscanf(fptr, "%s", N1);

  strtok(N1, "x");

  strcpy(N2, strtok(NULL, "x"));

  if(strcmp(N1, N2) == 0)
  {
    N = atoi(N1);
  }

  else
  {
    file_errors = 1;
    FILE *output = fopen("sudoku.out", "w");
    fprintf(output, "File out of format");
    fclose(output);
    fclose(fptr);
    exit(EXIT_FAILURE);
  }

  fscanf(fptr, "%s", A1);

  strtok(A1, "x");

  strcpy(B1, strtok(NULL, "x"));

  B = atoi(A1);

  A = atoi(B1);

  //acquiring sudoku matrix inputs
  int sudoku[N][N], numbers[N*N], j;
  char inputs[5];
  unsigned long input_count = 0, count = 0;
  
  while (fscanf(fptr, "%s", inputs) == 1)
  {
    numbers[input_count] = atoi(inputs);
    
    if (numbers[input_count] < 1 || numbers[input_count] > N)
    {
      file_errors = 1;
    }

    input_count += 1; //counting input total
  }
  
  fclose(fptr);
 
  count = 0;

  //inserting in sudoku matrix
  for(i = 0; i < N; i += 1)
  {
    for(j = 0; j < N; j += 1)
    {
      sudoku[i][j] = numbers[count];
      count += 1;
    }
  }

  ptr = &sudoku[0][0];

  //starting validations

  grid_size_validation(input_count, N);
  subgrid_size_validation(A, B, N);

  if (file_errors == 1)
  {
    FILE *output = fopen("sudoku.out", "w");
    fprintf(output, "File out of format");
    fclose(output);
    exit(EXIT_FAILURE);
  }

  pthread_t row[N], column[N], subgrid;

  datastruct ds[N];
  
  for (i = 0; i < N; i += 1)
  {
    ds[i].id = i;
    
    pthread_create(row + i, NULL, &row_validation, (void *)&ds[i]);
    pthread_create(column + i, NULL, &column_validation, (void *)&ds[i]);
  }

  pthread_create(&subgrid, NULL, &subgrid_validation, NULL);
  
  for (i = 0; i < N; i += 1)
  {
    pthread_join(row[i], NULL);
    pthread_join(column[i], NULL);
  }
  
  pthread_join(subgrid, NULL);

  if (sudoku_errors == 1)
  {
    FILE *output = fopen("sudoku_vag.out", "w");
    fprintf(output, "FAIL");
    fclose(output);
    exit(EXIT_FAILURE);
  }
  
  FILE *output = fopen("sudoku.out", "w");
  fprintf(output, "SUCCESS");
  fclose(output);
  return 0;
}

void grid_size_validation(unsigned long input_count, int N)
{
  if (input_count != N * N || N < 9 || N > 1000)
  {
    file_errors = 1;
    return;
  }
    
  else
  {
    return;
  }
}

void subgrid_size_validation(int A, int B, int N) //A * B == N?
{
  if (A * B != N || A < 0 || B < 0 || A > N || B > N)
  {
    file_errors = 1;
    return;
  }
    
  else
  {
    return;
  }
}

void *row_validation(void *data)
{
  datastruct *ds = (datastruct *)data;
  int *sudoku_ref = ptr;
  int *aux_ptr;

  sudoku_ref += N * ds->id;

  for (int i = 0; i < N; i += 1)
  {
    for (aux_ptr = sudoku_ref + i; aux_ptr < sudoku_ref + N; aux_ptr += 1)
    {
      if (aux_ptr != sudoku_ref + i)
      {
        if (*aux_ptr == *(sudoku_ref + i))
        {
          sudoku_errors = 1;
          return NULL;
        }
      }
    }
  }
  return NULL;
}

void *column_validation(void *data)
{
  datastruct *ds = (datastruct *)data;
  int *sudoku_ref = ptr;
  int *aux_ptr;

  sudoku_ref += ds->id;

  for (int i = 0; i < N; i += 1)
  {
    for (aux_ptr = sudoku_ref + N * i; aux_ptr < sudoku_ref + N * N; aux_ptr += N)
    {
      if (aux_ptr != sudoku_ref + N * i)
      {
        if (*aux_ptr == *(sudoku_ref + N * i))
        {
          sudoku_errors = 1;
          return NULL;
        }
      }
    }
  }
  return NULL;
}

void *subgrid_validation()
{
  int *sudoku_ref = ptr;
  int sudoku[N][N];

  for (int i = 0; i < N; i += 1)
  {
    for (int j = 0; j < N; j += 1)
    {
      sudoku[i][j] = *sudoku_ref;
      sudoku_ref += 1;
    }
  }
  
  int verification_count = 0;
  /* 
     A = subgrid x size
     B = subgrid y size
     i = subrid y coordinate
     j = subgrid x coordinate
     k = y coordinate inside subgrid
     l = x coordinate inside subgrid
  */

  //inside grid
  for (int i = 0; i < N; i += B)
  {
    for (int j = 0; j < N; j += A)
    {
      //inside subgrid
      
      for (int current_number = 1; current_number < N; current_number += 1)
      {  
        verification_count = 0;
        
        for (int k = i; k < i + B; k += 1)
        {
          for (int l = j; l < j + A; l += 1)
          {    
            if (sudoku[k][l] == current_number)
            {
              verification_count += 1;
            }
          }
        }
        if (verification_count != 1)
        {
          sudoku_errors = 1;
          return NULL;
        }
      }
    }
  }
  return NULL;
}
