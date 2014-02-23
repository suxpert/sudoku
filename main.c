#include<stdio.h>
#include"sudoku.h"

int main(int argc, char *argv[])
{
	FILE * fin = NULL;
	int lineCounter = 1;
	int sudoku[9][9] = {{0}};
	int row = 0;
	int col = 0;
	int fileOK = 1;
	int prec, c;

	if( 2 != argc ) {
		printf("Argument error!\nUsage: %s <file-name>\n", argv[0]);
		return 0;
	}

	fin = fopen(argv[1], "rb");
	if( NULL == fin ) {
		printf("Cannot open file `%s' for read!\n", argv[1]);
		return 1;
	}

#define GET_NEXT_CHAR (prec = c, c = fgetc(fin))

	while( fileOK && EOF != (GET_NEXT_CHAR) ) {
		// printf("`%c|%c',", prec, c);
		switch( c ) {
			case '#' :
				while( '\n' != (GET_NEXT_CHAR) );
				// fall through;
			case '\n' :
				if( 0 != col%9 ) {
					printf("Error! Unfinished line at L%d, %d %s expected!",
							lineCounter, (9-col),
							(9-col==1? "number": "numbers") );
					fileOK = 0;
				} // if unfinished
				lineCounter++;
				break;
			case '%' :
				if( '\n' == prec && '%' == (GET_NEXT_CHAR) ) {
					// line starting by `%%', treat as separator
					if( 0 != row%9 || 0 != col%9 ) {
						printf("Error!"
								" Incomplete sudoku before separator at L%d!",
								lineCounter);
						fileOK = 0;
					} // if incomplete
				} // if separator
				break;
			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9' :
				sudoku[row][col++] = c-'0';
				if( 9 == col ) {
					col = 0;
					row++;
					if( 9 == row ) {
						// a complete sudoku has been stored, try to solve it.
						printf(">> Complete sudoku, will now solve...\n");
						printSudoku(sudoku);
						sudokuSolve(sudoku);
						row = 0;
					} // if complete
				} // if line finished
				break;
			default :
				// do nothing;
				break;
		} // switch
	} // while
	return 0;
}

/* vim: set ts=4 sw=4 : */
