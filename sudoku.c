#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"sudoku.h"

void printSudoku( int sudoku[9][9] )
{
	int i, j;

	for( i=0 ; i<9 ; i++ ) {
		if( 0 == i%3 ) {
			printf("-------------\n");
		}
		for( j=0 ; j<9 ; j++ ) {
			if( 0 == j%3 ) {
				printf("|");
			}
			if( 0 == sudoku[i][j] ) {
				printf(" ");
			}
			else {
				printf("%1d", sudoku[i][j]);
			}
		}
		printf("|\n");
	}
	printf("-------------\n");
	
	return;
}

void dispAux(int aux[10][9][9])
{
	int i, j, k;
	for( i=0 ; i<9 ; i++ ) {
		for( j=0 ; j<9 ; j++ ) {
			printf("(%1d)", aux[0][i][j]);
			for( k=1 ; k<10 ; k++ )
			{
				printf("%1d", aux[k][i][j]);
			}
			printf(" ");
		}
		printf("\n");
	}
	printf("\n");

	system("pause");
	return;
}

int exclude(int sudoku[9][9], int aux[10][9][9], int row, int col, int num)
{
	int posi, posj, i, j;
	int noCandidate = 0;
	// int collision;
	int collision = 0;

	if( 1 == aux[num][row][col] ) {
		// printf(">>> The certain one is used.\n");
		collision = 1;
	}
	for( i=0 ; i<9 ; i++ ) {
		if( 0 == aux[num][row][i] ) {
			aux[num][row][i] = 1;
			aux[0][row][i]--;
			if( 0 == aux[0][row][i] && 0 == sudoku[row][i] ) {
				printf("Zero candidate? At (%d, %d)\n", row, i);
				noCandidate = 2;
			}
		}
		if( 0 == aux[num][i][col] ) {
			aux[num][i][col] = 1;
			aux[0][i][col]--;
			if( 0 == aux[0][i][col] && 0 == sudoku[i][col] ) {
				printf("Zero candidate? At (%d, %d)\n", i, col);
				noCandidate = 2;
			}
		}
	}

	posi = row/3*3;
	posj = col/3*3;
	for( i=posi ; i<posi+3 ; i++ ) {
		for( j=posj ; j<posj+3 ; j++ ) {
			if( 0 == aux[num][i][j] ) {
				aux[num][i][j] = 1;
				aux[0][i][j]--;
				if( 0 == aux[0][i][j] && 0 == sudoku[i][j] ) {
					printf("Zero candidate? At (%d, %d)\n", i, j);
					noCandidate = 2;
				}
			}
		}
	}

	if( 0 != collision ) {
		printf("Error! Number collision?\n");
	}
	if( 0 != aux[0][row][col] ) {
		// printf("Here should be zero\n");
		aux[0][row][col] = 0;
		for( i=1 ; i<10 ; i++ ) {
			aux[i][row][col] = 1;
		}
	}

	return collision+noCandidate;
}

int groupDetermine(int sudoku[9][9], int aux[10][9][9])
{
	int sudokuType;
	int blocki, blockj;
	int i, j, k;
	int counter[10];
	int numlist[10];

	for( blocki=0 ; blocki<9 ; blocki+=3 ) {
		for( blockj=0 ; blockj<9 ; blockj+=3 ) {
			// for every 3x3 block
			memset(counter, 0, 10*sizeof(int));
			memset(numlist, 0, 10*sizeof(int));
			for( i=blocki ; i<blocki+3 ; i++ ) {
				for( j=blockj ; j<blockj+3 ; j++ ) {
					if( 0 != sudoku[i][j] ) {
						numlist[ sudoku[i][j] ] = 1;
					}
					for( k=1 ; k<10 ; k++ ) {
						if( 0 == aux[k][i][j] ) {
							counter[k]++;
						}
					} // for the candidate list
				} // block at j
			} // block at i

			// check if there are errors
			for( k=1 ; k<10 ; k++ ) {
				if( 0 == numlist[k] && 0 == counter[k] ) {
					printf("Error! Block (%d, %d) has lost number %d.\n",
							blocki, blockj, k);
					return 2;
				}
			} // for the sum-list

			// check if there are group-unique number
			for( k=1 ; k<10 ; k++ ) {
				if( 1 == counter[k] ) {
					// in this group, only one `k' can be used, thus use it.
					for( i=blocki ; i<blocki+3 ; i++ ) {
						for( j=blockj ; j<blockj+3 ; j++ ) {
							if( 0 == aux[k][i][j] ) {
								// here we go...
								printf("Block determined, %d at (%d, %d)\n",
										k, i, j);
								sudoku[i][j] = k;
								sudokuType = exclude(sudoku, aux, i, j, k);
								return 0;
							} // find the coordinate
						} // block at j
					} // block at i
				} // if exist unique
			} // for the sum-list

		} // for every 3x3 block: i
	} // for every 3x3 block: j

	// if not found in blocks, try lines:
	// row
	for( i=0 ; i<9 ; i++ ) {
		memset(counter, 0, 10*sizeof(int));
		memset(numlist, 0, 10*sizeof(int));
		for( j=0 ; j<9 ; j++ ) {
			for( k=1 ; k<10 ; k++ ) {
				if( 0 != sudoku[i][j] ) {
					numlist[ sudoku[i][j] ] = 1;
				}
				if( 0 == aux[k][i][j] ) {
					counter[k]++;
				}
			} // for the candidate list

		} // for the whole row

		// check if there are errors
		for( k=1 ; k<10 ; k++ ) {
			if( 0 == numlist[k] && 0 == counter[k] ) {
				printf("Error! Row %d has lost number %d.\n",
						i, k);
				return 2;
			}
		} // for the sum-list

		// check if there are group-unique number
		for( k=1 ; k<10 ; k++ ) {
			if( 1 == counter[k] ) {
				for( j=0 ; j<9 ; j++ ) {
					if( 0 == aux[k][i][j] ) {
						// here we go...
						printf("Row determined, %d at (%d, %d)\n", k, i, j);
						sudoku[i][j] = k;
						sudokuType = exclude(sudoku, aux, i, j, k);
						return 0;
					} // find the coordinate
				}

			} // if exist unique
		} // for the sum-list
	} // for every row

	// column
	for( j=0 ; j<9 ; j++ ) {
		memset(counter, 0, 10*sizeof(int));
		memset(numlist, 0, 10*sizeof(int));
		for( i=0 ; i<9 ; i++ ) {
			for( k=1 ; k<10 ; k++ ) {
				if( 0 != sudoku[i][j] ) {
					numlist[ sudoku[i][j] ] = 1;
				}
				if( 0 == aux[k][i][j] ) {
					counter[k]++;
				}
			} // for the candidate list

		} // for the whole column

		// check if there are errors
		for( k=1 ; k<10 ; k++ ) {
			if( 0 == numlist[k] && 0 == counter[k] ) {
				printf("Error! Column %d has lost number %d.\n",
						j, k);
				return 2;
			}
		} // for the sum-list

		// check if there are group-unique number
		for( k=1 ; k<10 ; k++ ) {
			if( 1 == counter[k] ) {
				for( i=0 ; i<9 ; i++ ) {
					if( 0 == aux[k][i][j] ) {
						// here we go...
						printf("Column determined, %d at (%d, %d)\n", k, i, j);
						sudoku[i][j] = k;
						sudokuType = exclude(sudoku, aux, i, j, k);
						return 0;
					} // find the coordinate
				}

			} // if exist unique
		} // for the sum-list
	} // for every row

	return 1; // Not found, method failed.
}

int uniCandidate(int sudoku[9][9], int aux[10][9][9])
{
	int i, j, k;
	int sudokuType;

	for( i=0 ; i<9 ; i++ ) {
		for( j=0 ; j<9 ; j++ ) {
			if( 1 == aux[0][i][j] ) {
				for( k=1 ; k<10 ; k++ ) {
					if( 0 == aux[k][i][j] ) {
						// here we go...
						printf("Unique Candidate determined, %d at (%d, %d)\n",
								k, i, j);
						sudoku[i][j] = k;
						sudokuType = exclude(sudoku, aux, i, j, k);
						return 0;
					}
				}
			}
		}
	}

	return 1;
}


int sudokuSolve( int sudoku[9][9] )
{
	int aux[10][9][9]={{{0}}};
	int blanks = 81;

	int i, j, flag, sudokuType;
	for( i=0 ; i<9 ; i++ ) {
		for( j=0 ; j<9 ; j++ ) {
			aux[0][i][j] = 9;
		}
	} // initialize

	for( i=0 ; i<9 ; i++ ) {
		for( j=0 ; j<9 ; j++ ) {
			if( 0 != sudoku[i][j] ) {
				blanks--;
				sudokuType = exclude(sudoku, aux, i, j, sudoku[i][j]);
				if( 0 != sudokuType ) {
					printf("I think your sudoku is somewhere incorrect.\n");
				}
				// dispAux(aux);
			}
		}
	} // exclude the numbers already given.


	// solve the sudoku.
	while( blanks>0 ) {
		// printf("%d blanks left\n", blanks);
		// printf("Try the group determine method first\n");
		flag = groupDetermine(sudoku, aux);
		if( 0 == flag ) {
			blanks--;
			continue;
		}
		else{
			// printf("Group Determine fails, try next, Unique Candidate.\n");
		}
		flag = uniCandidate(sudoku, aux);
		if( 0 == flag ) {
			blanks--;
			continue;
		}
		else{
			printf("Unique Candidate fails, try next, ??.\n");
			blanks =0;
		}
	}

	// printf("");
	printSudoku(sudoku);

	return 0;
}

/* vim: set ts=4 sw=4 : */

