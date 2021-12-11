#include "chess.h"
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/random.h>

/*
#define DEVICE "/dev/chess"
#define BUF_SIZE PAGE_SIZE
#define INVFMT -2
#define UNKCMD -1
#define OK	0
#define CHECK   1
#define MATE	2
#define ILL 	3
#define OOT 	4
#define NOG	5
#define PRNTBRD	6
#define WHITE	0
#define BLACK	1
#define PAWN	0
#define ROOK	1
#define	KNIGHT	2
#define BISHOP	3
#define QUEEN	4
#define KING	5
*/
#define mem_size 1024
static int majorNumber;   	///< Stores the device number
static char message[256] = {0}; ///< Memory for the string that is passed from user space
static short  size_of_message;  ///< Used to remember the size of the string stored		
bool ready = false;
uint8_t *kernel_command_bufferer;

#define BUF_SIZE PAGE_SIZE

//struct chess_board *chss_brd_ptr;
int crnt_command_bufferer;
char board_display[129];

int p1_horizontal, p2_horizontal, p1_verticle, p2_verticle;
bool piece_captured, piece_promoted;
char piece_capturedPiece[2];
char piece_promotedPiece[2];

/*
// The Board struct itself
typedef struct chess_board{
	// True if player is white
	bool whitePlayers;
	bool whitePlayersTurn;
	char board[8][8][2];
	int outOfBoard[2][6];
	int CPUPieces[16][2];
	int LocationOfKing[2][2];
	bool cpuCheck;
	bool gameOver;
} chess_board;
*/
struct chess_board *chss_brd_ptr;

/*********************************************** The Rules for the 6 Pieces in Chess ***********************************************************/
/***********************************************************************************************************************************************/
bool status(const char *piece, struct chess_board *chss_brd_ptr){
	int color, type;	
	// find color
	if (piece[0] == 'W'){ 
		color = WHITE; 
	}
	else{ 
		color = BLACK; 
	}
	// find piece
	if (piece[1] == 'P'){ 
		type = PAWN; 
	}
	else if (piece[1] == 'R'){ 

		type = ROOK; 
	}



	else if (piece[1] == 'N'){ 
		type = KNIGHT; 
	}
	else if (piece[1] == 'B'){ 
		type = BISHOP; 
	}
	else if (piece[1] == 'Q'){ 
		type = QUEEN; 
	}



	else{ 
		type = KING; 
	}

	if (chss_brd_ptr->outOfBoard[color][type] == 0){ 
		return true; 
	}

	return false;
}

bool pawn(int p1_horizontal, int p1_verticle, int p2_horizontal, int p2_verticle, bool white_piece, bool predator, struct chess_board *chss_brd_ptr){
	if (predator){


		// move up
		if (white_piece){
			if (p2_verticle == p1_verticle + 1 && (p2_horizontal == p1_horizontal + 1 || p2_horizontal == p1_horizontal - 1)){ 
				return true; 
			}


			else{
				printk("ILL MOVE");
				return false;
			}
		}



		// move down
		else
		{
			if (p2_verticle == p1_verticle - 1 && (p2_horizontal == p1_horizontal + 1 || p2_horizontal == p1_horizontal - 1)){ 
				return true; 
			}


			else{
				printk("ILL MOVE");
				return false;
			}
		}
	}
	else if (p2_horizontal != p1_horizontal){ 
			printk("ILL MOVE: illegal move horizontally");
			return false; 
	}


	else{
		if (white_piece){
			// first move
			if (p1_verticle == 1){



				// move two spaces 
				if (p2_verticle == p1_verticle + 2){


					// blocked
					if (chss_brd_ptr->board[p1_horizontal][p1_verticle+1][0] == '*' && chss_brd_ptr->board[p1_horizontal][p2_verticle][0] == '*'){ 
						return true; 
					}
					else{					
						printk("ILL MOVE: blocke");
						return false;
					}
				}
			}
			// move one
			if (p2_verticle == p1_verticle + 1 && chss_brd_ptr->board[p1_horizontal][p2_verticle][0] == '*'){ 
				return true; 
			}


			else{
				printk("ILL MOVE");
				return false;
			}
		}
		// black 
		else{

			if (p1_verticle == 6){
				if (p2_verticle == p1_verticle - 2){


					if (chss_brd_ptr->board[p1_horizontal][p1_verticle-1][0] == '*' && chss_brd_ptr->board[p1_horizontal][p2_verticle][0] == '*'){ 
						return true; 
					}
					else{				

	
						printk("ILL MOVE: blocked");
						return false;
					}
				}
			}
			if (p2_verticle == p1_verticle - 1 && chss_brd_ptr->board[p1_horizontal][p2_verticle][0] == '*'){ 
				return true; 


			}
			else{
				printk("ILL MOVE");
				return false;
			}
		}
	}
	return false;
}

bool rook(int p1_horizontal, int p1_verticle, int p2_horizontal, int p2_verticle, bool white_piece, bool predator, struct chess_board *chss_brd_ptr){	
	//i and n == slot 1 and slot 2 or position 1 and 2
	int i, n;

	//first check horizontally and then check vertically!!!


	if (p1_horizontal != p2_horizontal && p1_verticle == p2_verticle){
		// increase or decrease slot?
		if (p2_horizontal > p1_horizontal){ 
			n = 1; 
		}


		else{ 
			n = -1; 
		}
		for (i = p1_horizontal + n; i != p2_horizontal; i = i + n){


			if (chss_brd_ptr->board[i][p1_verticle][0] != '*'){


				printk("ILL MOVE:block");
				return false;
			}
		}
		if (!predator && chss_brd_ptr->board[p2_horizontal][p2_verticle][0] != '*'){
			printk("ILL MOVE:block");
			return false;
		}

		return true;
	}
	if (p1_horizontal == p2_horizontal && p1_verticle != p2_verticle){


		if (p2_verticle > p1_verticle){ 
			n = 1; 
		}
		else{ 
			n = -1; 
		}

		// every slot
		for (i = p1_verticle + n; i != p2_verticle; i = i + n){


			if (chss_brd_ptr->board[p1_horizontal][i][0] != '*'){


				printk("ILL MOVE: blocks");
				return false;
			}
		}
		// Only checks the end space if not making a kill
		if (!predator && chss_brd_ptr->board[p2_horizontal][p2_verticle][0] != '*'){


			printk("ILL MOVE: block");
			return false;
		}
		return true;
	}
	return false;
}

bool knight(int p1_horizontal, int p1_verticle, int p2_horizontal, int p2_verticle, bool white_piece, bool predator, struct chess_board *chss_brd_ptr){
	// n and m represents pos1 and pos2 
	int n, m;
	
	// distance traveled
	n = abs(p2_horizontal - p1_horizontal);

	m = abs(p2_verticle - p1_verticle);



	// calculating combo jump


	if ((n == 2 && m == 1) || (n == 1 && m == 2)){

		if (!predator && chss_brd_ptr->board[p2_horizontal][p2_verticle][0] != '*'){


			printk("ILL MOVE: block");
			return false;
		}
		return true;
	}
	return false;
}

bool bishop(int p1_horizontal, int p1_verticle, int p2_horizontal, int p2_verticle, bool white_piece, bool predator, struct chess_board *chss_brd_ptr){
	
	//positions horizontally and vertically for p1 and p2
	int i, j, n, m;
	

	if (abs(p2_horizontal - p1_horizontal) != abs(p2_verticle - p1_verticle)){
		printk("ILL MOVE: hoizontal only");


		return false;
	}
	if (p2_horizontal > p1_horizontal){ 

		n = 1; 
	}
	else{ 
		n = -1; 
	}
	if (p2_verticle > p1_verticle){ 

		m = 1; 
	}
	else{ 
		m = -1; 
	}

	j = p1_verticle + m;
	for (i = p1_horizontal + n; i != p2_horizontal; i = i + n){


		if (chss_brd_ptr->board[i][j][0] != '*'){
			printk("ILL MOVE: block");

			return false;
		}
		j = j + m;
	}

	if (!predator && chss_brd_ptr->board[p2_horizontal][p2_verticle][0] != '*'){
		printk("ILL MOVE: block");

		return false;
	}
	return true;
}

bool queen(int p1_horizontal, int p1_verticle, int p2_horizontal, int p2_verticle, bool white_piece, bool predator, struct chess_board *chss_brd_ptr){
	bool canMove = false;	

	
	if (abs(p2_horizontal - p1_horizontal) == abs(p2_verticle - p1_verticle)){



		// diagonal move from bishop
		canMove = bishop(p1_horizontal, p1_verticle, p2_horizontal, p2_verticle, white_piece, predator, chss_brd_ptr);
	}
	else
	{



		// hor./vert. move from rook
		canMove = rook(p1_horizontal, p1_verticle, p2_horizontal, p2_verticle, white_piece, predator, chss_brd_ptr);
	}
	return canMove;
}

bool king(int p1_horizontal, int p1_verticle, int p2_horizontal, int p2_verticle, bool white_piece, bool predator, struct chess_board *chss_brd_ptr){
	

	
	//moves one space in a square rotaion
	int n, m;

	n = abs(p2_horizontal - p1_horizontal);
	m = abs(p2_verticle - p1_verticle);
	if ((n != 0 && n != 1) || (m != 0 && m != 1)){


		printk("ILL MOVE: 1 slot at a time");
		return false;
	}
	chss_brd_ptr->board[p1_horizontal][p1_verticle][0] = '*';

	chss_brd_ptr->board[p1_horizontal][p1_verticle][1] = '*';

	if (CheckForCheck(p2_horizontal, p2_verticle, white_piece, chss_brd_ptr)){


		printk("ILL MOVE: deliberate check");
		if (white_piece){ 
			chss_brd_ptr->board[p1_horizontal][p1_verticle][0] = 'W'; 


		}
		else{ 
			chss_brd_ptr->board[p1_horizontal][p1_verticle][0] = 'B'; 


		}
		chss_brd_ptr->board[p1_horizontal][p1_verticle][1] = 'K';
		return false;
	}
	if (white_piece){ 
		chss_brd_ptr->board[p1_horizontal][p1_verticle][0] = 'W'; 


	}
	else{ 
		chss_brd_ptr->board[p1_horizontal][p1_verticle][0] = 'B'; 
	}


	chss_brd_ptr->board[p1_horizontal][p1_verticle][1] = 'K';

	if (!predator && chss_brd_ptr->board[p2_horizontal][p2_verticle][0] != '*'){
		printk("ILL MOVE: block");
		return false;
	}
	return true;
}

bool CheckForCheck(int x, int y, bool white_piece, struct chess_board *chss_brd_ptr){
	int i, j, n, m, c;

	char compare_with;

	if (white_piece){ 

		compare_with = 'B'; 
	}
	else{ 
		compare_with = 'W'; 
	}

	if (x <= 6 && y <= 5 && chss_brd_ptr->board[x+1][y+2][0] == compare_with && chss_brd_ptr->board[x+1][y+2][1] == 'N'){ return true; }


	if (x <= 5 && y <= 6 && chss_brd_ptr->board[x+2][y+1][0] == compare_with && chss_brd_ptr->board[x+2][y+1][1] == 'N'){ return true; }
	if (x <= 5 && y >= 1 && chss_brd_ptr->board[x+2][y-1][0] == compare_with && chss_brd_ptr->board[x+2][y-1][1] == 'N'){ return true; }
	if (x <= 6 && y >= 2 && chss_brd_ptr->board[x+1][y-2][0] == compare_with && chss_brd_ptr->board[x+1][y-2][1] == 'N'){ return true; }


	if (x >= 1 && y >= 2 && chss_brd_ptr->board[x-1][y-2][0] == compare_with && chss_brd_ptr->board[x-1][y-2][1] == 'N'){ return true; }
	if (x >= 2 && y >= 1 && chss_brd_ptr->board[x-2][y-1][0] == compare_with && chss_brd_ptr->board[x-2][y-1][1] == 'N'){ return true; }

	if (x >= 2 && y <= 6 && chss_brd_ptr->board[x-2][y+1][0] == compare_with && chss_brd_ptr->board[x-2][y+1][1] == 'N'){ return true; }

	if (x >= 1 && y <= 5 && chss_brd_ptr->board[x-1][y+2][0] == compare_with && chss_brd_ptr->board[x-1][y+2][1] == 'N'){ return true; }
	for (i = -1; i <= 1; i++){

		for (j = -1; j <= 1; j++){


			n = x + i;
			m = y + j;

			c = 1;
			if (i != 0 || j != 0){


				// until end of board or reaches a piece
				while (n >= 0 && n <= 7 && m >= 0 && m <= 7 && chss_brd_ptr->board[n][m][0] == '*'){
					n = n + i;


					m = m + j;
					c++;
				}
				// is king reachable?
				if (n >= 0 && n <= 7 && m >= 0 && m <= 7 && chss_brd_ptr->board[n][m][0] == compare_with){
					if (chss_brd_ptr->board[n][m][1] == 'P'){	


						if (compare_with == 'B'){
							if (i != 0 && j == 1 && c == 1){
								printk("BP check [%i,%i]", n, m);


								return true;
							}
						}
						else{
							if (i != 0 && j == -1 && c == 1){

								printk("WP check [%i,%i]", n, m);
								return true;
							}
						}
					}
					else if (chss_brd_ptr->board[n][m][1] == 'R'){


						if (i == 0 || j == 0){

							printk("rook check [%i,%i]", n, m);
							return true;
						}
					}
					else if (chss_brd_ptr->board[n][m][1] == 'B'){

						if (i != 0 && i != 0){
							printk("bishop check [%i,%i]", n, m);
							return true;
						}
					}
					else if (chss_brd_ptr->board[n][m][1] == 'Q'){


						printk("queen check [%i,%i]", n, m);
						return true;
					}
					else{
						if (c == 1){ 	
							printk("king check [%i,%i]", n, m);
							return true; 
						}
					}
				}
			}
		}
	}
	return false;
}

bool CheckForMate(int x, int y, bool white_piece, struct chess_board *chss_brd_ptr){


	int i, j, n, m;
	bool isMate = true;
	char color;

	if (white_piece){ 
		color = 'W'; 
	}
	else{ 
		color = 'B'; 
	}
	chss_brd_ptr->board[x][y][0] = '*';
	chss_brd_ptr->board[x][y][1] = '*';
	for (i = -1; i <= 1; i++){


		for (j = -1; j <= 1; j++){
			if (i != 0 || j != 0){


				n = x + i;
				m = y + j;
	
				if (n >= 0 && n <= 7 && m >= 0 && m <= 7 && chss_brd_ptr->board[n][m][0] != color){ 


					if (!CheckForCheck(n, m, white_piece, chss_brd_ptr)){ 
						isMate = false; 
					}
				}
			}
		}
	}
	chss_brd_ptr->board[x][y][0] = color;

	chss_brd_ptr->board[x][y][1] = 'K';
	return isMate;
}

/*******************************************************************************************************************/
static void Game_Over(void){
	//04
	chss_brd_ptr->gameOver = true;
	printk(KERN_INFO "end game....\n");
}

static bool CPUgetValidMove(struct chess_board *chss_brd_ptr, int *move, char color){
	char type;


	int x, y, i, r, r2;
	int numAxes, dist;
	int axes[8][2];
	
	x = move[0];

	y = move[1];
	type = chss_brd_ptr->board[x][y][1];

	numAxes = 0;
	if (type == 'P'){

		if (color == 'W'){


			printk("move_cpu_piece");
			if (x >= 1 && y <= 6 && chss_brd_ptr->board[x-1][y+1][0] == 'B'){
				axes[numAxes][0] = -1;
				axes[numAxes][1] = 1;

				numAxes++;
			}
			if (x <= 6 && y <= 6 && chss_brd_ptr->board[x+1][y+1][0] == 'B'){


				axes[numAxes][0] = 1;
				axes[numAxes][1] = 1;
				numAxes++;
			}
			if (y <= 6 && chss_brd_ptr->board[x][y+1][0] == '*'){
				axes[numAxes][0] = 0;

				axes[numAxes][1] = 1;

				numAxes++;
			}
			if (y == 1 && chss_brd_ptr->board[x][2][0] == '*' && chss_brd_ptr->board[x][3][0] == '*'){
				axes[numAxes][0] = 0;

				axes[numAxes][1] = 2;

				numAxes++;
			}
		}
		else{
			printk("move_cpu_piece");


			if (x >= 1 && y >= 1 && chss_brd_ptr->board[x-1][y-1][0] == 'W'){
				axes[numAxes][0] = -1;

				axes[numAxes][1] = -1;
				numAxes++;
			}
			if (x <= 6 && y >= 1 && chss_brd_ptr->board[x+1][y-1][0] == 'W'){
				axes[numAxes][0] = 1;


				axes[numAxes][1] = -1;
				numAxes++;
			}
			if (y >= 1 && chss_brd_ptr->board[x][y-1][0] == '*'){
				axes[numAxes][0] = 0;

				axes[numAxes][1] = -1;
				numAxes++;
			}
			if (y == 6 && chss_brd_ptr->board[x][5][0] == '*' && chss_brd_ptr->board[x][4][0] == '*'){
				axes[numAxes][0] = 0;

				axes[numAxes][1] = -2;
				numAxes++;
			}
		}
		if (numAxes > 0){
			r = get_random_int() % numAxes;
			move[2] = move[0] + axes[r][0];

			move[3] = move[1] + axes[r][1];
			return true;

		}
		return false;
	}
	else if (type == 'N'){
		printk("move_cpu_piece: knight");
	}
	else{
		numAxes = 0;
		if (type != 'B'){
			axes[numAxes][0] = 0;
			axes[numAxes][1] = 1;

			numAxes++; 
			axes[numAxes][0] = 1;

			axes[numAxes][1] = 0;
			numAxes++;
			axes[numAxes][0] = 0;

			axes[numAxes][1] = 1;
			numAxes++;
			axes[numAxes][0] = 1;

			axes[numAxes][1] = 0;
			numAxes++;
		}
		if (type != 'R'){
			axes[numAxes][0] = 1;
			axes[numAxes][1] = 1;

			numAxes++; 
			axes[numAxes][0] = 1;
			axes[numAxes][1] = -1;

			numAxes++;
			axes[numAxes][0] = -1;
			axes[numAxes][1] = -1;


			numAxes++;
			axes[numAxes][0] = -1;

			axes[numAxes][1] = 1;
			numAxes++;
		}

		// random axis
		r = get_random_int() % numAxes;

		if (type == 'K'){
			for (i = 0; i < numAxes; i++){
				//do until valid move

				chss_brd_ptr->board[x][y][0] = '*';
				chss_brd_ptr->board[x][y][1] = '*';

				if (x+axes[r][0] >= 0 && x+axes[r][0] <= 7 && y+axes[r][1] >= 0 && y+axes[r][1] <= 7 
					&& chss_brd_ptr->board[x+axes[r][0]][y+axes[r][1]][0] != color
					&& !CheckForCheck(x+axes[r][0], y+axes[r][1], color == 'W', chss_brd_ptr)){

					move[2] = move[0] + axes[r][0];
					move[3] = move[1] + axes[r][1];

					printk("[%i,%i]\n", axes[r][0], axes[r][1]);


					chss_brd_ptr->board[x][y][0] = color;
					chss_brd_ptr->board[x][y][1] = 'K';
					return true;
				}
				else{				
					r++;
					if (r >= numAxes){ 
						r = 0; 
					}
				}
				chss_brd_ptr->board[x][y][0] = color;

				chss_brd_ptr->board[x][y][1] = 'K';
			}
		}
		//bishop, queen or rook
		else
		{
			for (i = 0; i < numAxes; i++)
			{
				int dist = 0;
				x = x + axes[r][0];
				y = y + axes[r][1];
				while (x >= 0 && x <= 7 && y >= 0 && y <= 7 && chss_brd_ptr->board[x][y][0] == '*'){
					x = x + axes[r][0];
					y = y + axes[r][1];
					dist++;
				}
				if (x >= 0 && x <= 7 && y >= 0 && y <= 7 && chss_brd_ptr->board[x][y][0] != color){ 
					dist++; 
				}
				if (dist > 0){
					r2 = (get_random_int() % dist) + 1;
					move[2] = move[0] + (axes[r][0] * r2);
					move[3] = move[1] + (axes[r][1] * r2);
					return true;
				}
				else{
					r++;
					if (r >= numAxes){ 
						r = 0; 
					}
				}
			}
		}
	}

	return false;
}


int move_player(struct chess_board *chss_brd_ptr, const char *command_buffer){
	int horizontal_king, vertical_king;
	bool kingWhite;
	p1_horizontal = command_buffer[5] - 97;

	p1_verticle = command_buffer[6] - 49;


	p2_verticle = command_buffer[9] - 49;
	p2_horizontal = command_buffer[8] - 97;

	if (strlen(command_buffer) > 11){
		if (command_buffer[13] == 'y'){


			if (command_buffer[14] != command_buffer[3]){ 
				return ILLMOVE; 
			}


			piece_promoted = true;
			piece_promotedPiece[0] = command_buffer[14];


			piece_promotedPiece[1] = command_buffer[15];
		}
		else if (command_buffer[10] == 'y'){


			if (command_buffer[11] != command_buffer[3]){ 
				return ILLMOVE; 
			}
			piece_promoted = true;
			piece_promotedPiece[0] = command_buffer[11];


			piece_promotedPiece[1] = command_buffer[12];
		}
		else piece_promoted = false;
		if (command_buffer[13] == 'x'){


			if (command_buffer[14] == '*'){ 
				return ILLMOVE; 
			}
			if (command_buffer[14] == command_buffer[3]){ 
				return ILLMOVE; 
			}
			piece_captured = true;


			piece_capturedPiece[0] = command_buffer[14];
			piece_capturedPiece[1] = command_buffer[15];
		}
		else if (command_buffer[10] == 'x'){

			if (command_buffer[11] == '*'){ 
				return ILLMOVE; 
			}
			if (command_buffer[11] == command_buffer[3]){ 
				return ILLMOVE; 
			}
			piece_captured = true;

			piece_capturedPiece[0] = command_buffer[11];

			piece_capturedPiece[1] = command_buffer[12];
		}
		else piece_captured = false;

	}
	else{
		piece_promoted = false;
		piece_captured = false;
	}

	// legal move?
	if ( (p1_verticle == p2_verticle) && (p1_horizontal == p2_horizontal) ){

		printk("ILLMOVE: no move");
		return ILLMOVE;
	}


	if ((command_buffer[3] == 'W' && !chss_brd_ptr->whitePlayers) || (command_buffer[3] == 'B' && chss_brd_ptr->whitePlayers)){
		printk("ILLMOVE: not your player");

		return ILLMOVE;
	}
	if (command_buffer[3] != chss_brd_ptr->board[p1_horizontal][p1_verticle][0] || command_buffer[4] != chss_brd_ptr->board[p1_horizontal][p1_verticle][1]){
		printk("ILLMOVE: not at starting");

		return ILLMOVE;
	}
	if (piece_captured){
		if (piece_capturedPiece[1] != chss_brd_ptr->board[p2_horizontal][p2_verticle][1] || piece_capturedPiece[0] != chss_brd_ptr->board[p2_horizontal][p2_verticle][0]){ 

				printk("ILLMOVE");
				return ILLMOVE;
			}
	}
	if (piece_promoted){
		if (!((command_buffer[3] == 'W' && p2_verticle == 7) || command_buffer[4] != 'P' || (command_buffer[3] == 'B' && p2_verticle == 0))){ 
				printk("ILLMOVE: not pawn/edge of board");
				return ILLMOVE;
			}
		if (status(piece_promotedPiece, chss_brd_ptr)){


			printk("ILLMOVE");
			return ILLMOVE;
		}
	}
	if (command_buffer[4] == 'Q'){
		if (!queen(p1_horizontal, p1_verticle, p2_horizontal, p2_verticle, command_buffer[3] == 'W', piece_captured, chss_brd_ptr)){ 
			return ILLMOVE; 

		}
	}
	else if (command_buffer[4] == 'P'){
		if (!pawn(p1_horizontal, p1_verticle, p2_horizontal, p2_verticle, command_buffer[3] == 'W', piece_captured, chss_brd_ptr)){ 
			return ILLMOVE; 


		}
	}
	else if (command_buffer[4] == 'N'){
		if (!knight(p1_horizontal, p1_verticle, p2_horizontal, p2_verticle, command_buffer[3] == 'W', piece_captured, chss_brd_ptr)){ 
			return ILLMOVE; 

		}
	}
	else if (command_buffer[4] == 'R'){

		if (!rook(p1_horizontal, p1_verticle, p2_horizontal, p2_verticle, command_buffer[3] == 'W', piece_captured, chss_brd_ptr)){ 
			return ILLMOVE; 
		}
	}
	else if (command_buffer[4] == 'B'){

		if (!bishop(p1_horizontal, p1_verticle, p2_horizontal, p2_verticle, command_buffer[3] == 'W', piece_captured, chss_brd_ptr)){ 
			return ILLMOVE; 
		}
	}
	else{
		if (!king(p1_horizontal, p1_verticle, p2_horizontal, p2_verticle, command_buffer[3] == 'W', piece_captured, chss_brd_ptr)){ 
			
			return ILLMOVE; 
		}
	}
	if (piece_captured){
		int i, j;
		i = 0;
		if (piece_capturedPiece[0] == 'B') i = 1;

		if (piece_capturedPiece[1] == 'P') j = PAWN; 
		else if (piece_capturedPiece[1] == 'R') j = ROOK;

		else if (piece_capturedPiece[1] == 'N') j = KNIGHT;

		else if (piece_capturedPiece[1] == 'B') j = BISHOP; 
		else if (piece_capturedPiece[1] == 'Q') j = QUEEN; 
		else{ 
			j = KING;
			chss_brd_ptr->gameOver = true;
		}
		chss_brd_ptr->outOfBoard[i][j]++;
	}
	int i, j;
	if (piece_promoted){
		chss_brd_ptr->board[p2_horizontal][p2_verticle][0] = piece_promotedPiece[0];

		chss_brd_ptr->board[p2_horizontal][p2_verticle][1] = piece_promotedPiece[1];
		
		i = 0;
		if (piece_promotedPiece[0] == 'B') i = 1;
		if (piece_promotedPiece[1] == 'P')j = PAWN; 

		else if (piece_promotedPiece[1] == 'R') j = ROOK;	
		else if (piece_promotedPiece[1] == 'N') j = KNIGHT; 

		else if (piece_promotedPiece[1] == 'B') j = BISHOP; 
		else if (piece_promotedPiece[1] == 'Q') j = QUEEN; 
		else j = KING; 
		chss_brd_ptr->outOfBoard[i][j]--;
	}
	else{
		chss_brd_ptr->board[p2_horizontal][p2_verticle][0] = chss_brd_ptr->board[p1_horizontal][p1_verticle][0];

		chss_brd_ptr->board[p2_horizontal][p2_verticle][1] = chss_brd_ptr->board[p1_horizontal][p1_verticle][1];
	}
	chss_brd_ptr->board[p1_horizontal][p1_verticle][0] = '*';

	chss_brd_ptr->board[p1_horizontal][p1_verticle][1] = '*';

	if (command_buffer[4] == 'K'){
		if (command_buffer[3] == 'W'){
			chss_brd_ptr->LocationOfKing[0][0] = p2_horizontal; // white x

			chss_brd_ptr->LocationOfKing[0][1] = p2_verticle; // white y
		}
		else{
			chss_brd_ptr->LocationOfKing[1][0] = p2_horizontal; // black x

			chss_brd_ptr->LocationOfKing[1][1] = p2_verticle; // black y
		}
	}

	// Check for CHECK / MATE
	if (command_buffer[3] == 'W'){
		horizontal_king = chss_brd_ptr->LocationOfKing[1][0];

		vertical_king = chss_brd_ptr->LocationOfKing[1][1];
		kingWhite = false;
	}
	else{
		horizontal_king = chss_brd_ptr->LocationOfKing[0][0];

		vertical_king = chss_brd_ptr->LocationOfKing[0][1];
		kingWhite = true;
	}
	if (CheckForCheck(horizontal_king, vertical_king, kingWhite, chss_brd_ptr)){

		if (CheckForMate(horizontal_king, vertical_king, kingWhite, chss_brd_ptr)) return MATE;
		else return CHECK; 
	}
	return OK;
}

static bool get_correct_piece(struct chess_board *chss_brd_ptr, int *move, char color){
	int i, j, numPieces, c, r;

	int pieces[16][2];
	numPieces = 0;
	c = 0;
	for (i = 0; i <= 7; i++){
		for (j = 0; j <= 7; j++){

			if (chss_brd_ptr->board[i][j][0] == color){


				pieces[numPieces][0] = i;
				pieces[numPieces][1] = j;

				numPieces++;
			}
		}
	}
	r = get_random_int() % numPieces;
	while (true){


		move[0] = pieces[r][0];
		move[1] = pieces[r][1];

		if (CPUgetValidMove(chss_brd_ptr, move, color)) return true;
	
		r++;
		if (r >= numPieces) r = 0; 

		if (c >= numPieces) return false;
		c++;
	}
}

int move_cpu_piece(struct chess_board *chss_brd_ptr){

	int horizontal_king, vertical_king;
	bool kingWhite;
	char color;

	int move[4];
	if (chss_brd_ptr->whitePlayers) color = 'B';

	else color = 'W'; 
	if (chss_brd_ptr->cpuCheck){
		if (color == 'W'){

			move[0] = chss_brd_ptr->LocationOfKing[0][0];

			move[1] = chss_brd_ptr->LocationOfKing[0][1];
		}
		else{
			move[0] = chss_brd_ptr->LocationOfKing[1][0];

			move[1] = chss_brd_ptr->LocationOfKing[1][1];
		}
		CPUgetValidMove(chss_brd_ptr, move, color);

		chss_brd_ptr->cpuCheck = false;
	}
	else{
		get_correct_piece(chss_brd_ptr, move, color);
	}

	printk("[%i,%i]", move[2], move[3]);

	// piece_capturedtures piece if applicable
	if (chss_brd_ptr->board[move[2]][move[3]][0] != '*'){
		int i, j;

		i = 1;
		if (chss_brd_ptr->whitePlayers) i = 0;
		if (chss_brd_ptr->board[move[2]][move[3]][1] == 'P')

			{ j = PAWN; }
		else if (chss_brd_ptr->board[move[2]][move[3]][1] == 'R')
			{ j = ROOK;	}
		else if (chss_brd_ptr->board[move[2]][move[3]][1] == 'N')
			{ j = KNIGHT; }




		else if (chss_brd_ptr->board[move[2]][move[3]][1] == 'B')
			{ j = BISHOP; }

		else if (chss_brd_ptr->board[move[2]][move[3]][1] == 'Q')

			{ j = QUEEN; }
		else
		{ 
			j = KING;
			chss_brd_ptr->gameOver = true;
		}
		chss_brd_ptr->outOfBoard[i][j]++;

	}
	int c, i, r;
	char type; 


	int p1_horizontal, p2_horizontal, p1_verticle, p2_verticle;
	
	// coordinates
	p1_horizontal = move[0];
	p1_verticle = move[1];
	p2_horizontal = move[2];
	p2_verticle = move[3];



	if (move[3] == 0 || move[3] == 7){
		if (chss_brd_ptr->whitePlayers){ 
			c = 1;
			color = 'B';
		}
		else{
			c = 0;
			color = 'W';
		}
		i = 0;
		r = get_random_int() % 6;
		while (i < 6 && chss_brd_ptr->outOfBoard[c][r] == 0){
			r++;
			if (r >= 6) r = 0;
			i++;
		}
		chss_brd_ptr->outOfBoard[i][r]--;


		if (r == PAWN) type = 'P'; 
		else if (r == ROOK) type = 'R'; 
		else if (r == KNIGHT)type = 'N';
		else if (r == BISHOP) type = 'B';


		else if (r == QUEEN) type = 'Q'; 
		else type = 'K'; 

		chss_brd_ptr->board[p2_horizontal][p2_verticle][0] = color;


		chss_brd_ptr->board[p2_horizontal][p2_verticle][1] = type;
	}
	else{
		chss_brd_ptr->board[p2_horizontal][p2_verticle][0] = chss_brd_ptr->board[p1_horizontal][p1_verticle][0];

		chss_brd_ptr->board[p2_horizontal][p2_verticle][1] = chss_brd_ptr->board[p1_horizontal][p1_verticle][1];
	}

	chss_brd_ptr->board[p1_horizontal][p1_verticle][0] = '*';

	chss_brd_ptr->board[p1_horizontal][p1_verticle][1] = '*';

	if (chss_brd_ptr->board[move[2]][move[3]][1] == 'K'){
		if (color == 'W'){
			chss_brd_ptr->LocationOfKing[0][0] = move[2];
			chss_brd_ptr->LocationOfKing[0][1] = move[3];


		}
		else{


			chss_brd_ptr->LocationOfKing[1][0] = move[2];
			chss_brd_ptr->LocationOfKing[1][1] = move[3];
		}
	}

	// Check for CHECK / MATE
	if (color == 'W'){
		horizontal_king = chss_brd_ptr->LocationOfKing[1][0];


		vertical_king = chss_brd_ptr->LocationOfKing[1][1];
		kingWhite = false;
	}
	else{
		horizontal_king = chss_brd_ptr->LocationOfKing[0][0];


		vertical_king = chss_brd_ptr->LocationOfKing[0][1];
		kingWhite = true;
	}
	if (CheckForCheck(horizontal_king, vertical_king, kingWhite, chss_brd_ptr)){


		if (CheckForMate(horizontal_king, vertical_king, kingWhite, chss_brd_ptr)) return MATE; 
		else return CHECK; 
	}
	return OK;
}



/**********************read && write**************************/
//in open return 0 if successful
int chess_open(struct inode *pinode, struct file *pfile){	

	/* Creating Physical Memory for board*/
	chss_brd_ptr = kmalloc(sizeof(chess_board), GFP_KERNEL);
	if(chss_brd_ptr){
		printk(KERN_INFO "Device File opened...\n");
		return 0;
	}
	else{
		printk(KERN_ERR "Cannot allocate the memory to the kernel..\n");
		return -1;
	}

	return 0;
}

//in read return how many bytes you read, initially return 0
//for chess game, read is when you are sending it to the viewer && returns length
ssize_t chess_read(struct file *pfile, char __user *command_bufferer, size_t length, loff_t *offset)
{
	int i, j, k;

	printk(KERN_INFO "Data read : DONE...\n");

	if (crnt_command_bufferer == ILLMOVE){
		printk("ILLMOVE\n");
		copy_to_user(command_bufferer, "ILLMOVE\n", 8);


		length = 8;
	}

	else if (crnt_command_bufferer == OOT){
		printk("OOT\n");


		copy_to_user(command_bufferer, "OOT\n", 4);
		length = 4;
	}
	else if (crnt_command_bufferer == NOGAME){


		printk("NOGAME\n");
		copy_to_user(command_bufferer, "NOGAME\n", 7);

		length = 7;
	}
	else if (crnt_command_bufferer == CHECK){
		printk("CHECK\n");
		copy_to_user(command_bufferer, "CHECK\n", 6);

		length = 4;
	}
	else if (crnt_command_bufferer == MATE){
		printk("MATE\n");
		copy_to_user(command_bufferer, "MATE\n", 5);

		length = 4;
	}
	else if (crnt_command_bufferer == OK){

		printk("OK\n");

		copy_to_user(command_bufferer, "OK\n", 3);

		length = 3;
	}
	else if (crnt_command_bufferer == INVFMT){
		printk("INVFMT\n");
		copy_to_user(command_bufferer, "INVFMT\n", 7);

		length = 7;
	}
	else if (crnt_command_bufferer == UNKCMD){
		printk("UNKCMD\n");

		copy_to_user(command_bufferer, "UNKCMD\n", 7);
		length = 7;
	}

	/****************print board*************************/
	else if (crnt_command_bufferer == PRNTBRD){
		printk("PRINT BOARD\n");

		k = 0;

		for (j = 0; j < 8; j++){
			for (i = 0; i < 8; i++){
				board_display[k] = chss_brd_ptr->board[i][j][0];;
				k++;

				board_display[k] = chss_brd_ptr->board[i][j][1];
				k++;
			}
		}

		board_display[128] = '\n';
		length = 129;
		copy_to_user(command_bufferer, board_display, 129);

	}
	/****************print board*************************/
	else{
		printk("return -1 ...\n");
		return -1;
	}

	return length;
}

//in write you have to tell them how many bytes you are writing, initially return length
//for chess game, write is when you are getting stuff from the viewer
//HOLDS THE HUGEST CHUNK OF CODE FOR THIS CHESS ENGINE...in a way it is like the "main" function
ssize_t chess_write(struct file *pfile, const char __user *command_bufferer, size_t length, loff_t *offset)
{
	char command_buffer[length];
	copy_from_user(command_buffer, command_bufferer, length);

	printk(KERN_INFO "in write.....\n");

	// 00 == New Game
	if ((command_buffer[0] == '0') && (command_buffer[1] == '0')){

		if (command_buffer[2] == ' ' &&  command_buffer[3] == 'B' &&  command_buffer[4] == '\n'){
			//00
			int i, j;
			chss_brd_ptr->whitePlayers = false;
			chss_brd_ptr->whitePlayersTurn = true;
			//create board
			for (i = 0; i < 8; i++){
				for (j = 0; j < 8; j++){
			




					//player
					if (j == 0){
						chss_brd_ptr->board[i][j][0] = 'W';
						if (i == 3) chss_brd_ptr->board[i][j][1] = 'Q'; // Queen
						else if (i == 2 || i == 5) chss_brd_ptr->board[i][j][1] = 'B'; // Bishop

						else if (i == 1 || i == 6) chss_brd_ptr->board[i][j][1] = 'N'; // Knight
						else if (i == 0 || i == 7) chss_brd_ptr->board[i][j][1] = 'R'; // Rook
						else chss_brd_ptr->board[i][j][1] = 'K'; // King

					}



					// CPU
					else if (j == 7){
						chss_brd_ptr->board[i][j][0] = 'B';

						if (i == 3) chss_brd_ptr->board[i][j][1] = 'Q'; // Queen
						else if (i == 2 || i == 5) chss_brd_ptr->board[i][j][1] = 'B'; // Bishop
						else if (i == 1 || i == 6) chss_brd_ptr->board[i][j][1] = 'N'; // Knight

						else if (i == 0 || i == 7) chss_brd_ptr->board[i][j][1] = 'R'; // Rook
						else chss_brd_ptr->board[i][j][1] = 'K'; // King

					}




					// CPU
					else if (j == 6){
						chss_brd_ptr->board[i][j][0] = 'B';
						chss_brd_ptr->board[i][j][1] = 'P';
					}



					// Player
					else if (j == 1){
						chss_brd_ptr->board[i][j][0] = 'W';
						chss_brd_ptr->board[i][j][1] = 'P';
					}
					//empty pieces
					else{
						chss_brd_ptr->board[i][j][0] = '*';
						chss_brd_ptr->board[i][j][1] = '*';
					}
				}
			}

			for (i = 0; i < 6; i++){
				chss_brd_ptr->outOfBoard[0][i] = 0;
				chss_brd_ptr->outOfBoard[1][i] = 0;
			}
			//x cordinate
			chss_brd_ptr->LocationOfKing[0][0] = 4; // white x

			chss_brd_ptr->LocationOfKing[1][0] = 4; // black x
			//y cordinate 
			chss_brd_ptr->LocationOfKing[0][1] = 0; // white y

			chss_brd_ptr->LocationOfKing[1][1] = 7; // black y
			chss_brd_ptr->gameOver = false;

		}
					
		
		else if ((command_buffer[2] == ' ') && (command_buffer[3] == 'W') &&  command_buffer[4] == '\n'){
			 
			//00
			int i, j;

			// True if player is white
			chss_brd_ptr->whitePlayers = true;

			// White always starts
			chss_brd_ptr->whitePlayersTurn = true;

			// Initilizes Board
			for (i = 0; i < 8; i++){
				for (j = 0; j < 8; j++){




					// Player
					if (j == 0){
						chss_brd_ptr->board[i][j][0] = 'W';

						if (i == 3) chss_brd_ptr->board[i][j][1] = 'Q'; // Queen
						else if (i == 2 || i == 5) chss_brd_ptr->board[i][j][1] = 'B'; // Bishop
						else if (i == 1 || i == 6) chss_brd_ptr->board[i][j][1] = 'N'; // Knight

						else if (i == 0 || i == 7) chss_brd_ptr->board[i][j][1] = 'R'; // Rook
						else chss_brd_ptr->board[i][j][1] = 'K'; // King

					}



					// CPU
					else if (j == 7){
						chss_brd_ptr->board[i][j][0] = 'B';

						if (i == 3) chss_brd_ptr->board[i][j][1] = 'Q'; // Queen
						else if (i == 2 || i == 5) chss_brd_ptr->board[i][j][1] = 'B'; // Bishop
						else if (i == 1 || i == 6) chss_brd_ptr->board[i][j][1] = 'N'; // Knight

						else if (i == 0 || i == 7) chss_brd_ptr->board[i][j][1] = 'R'; // Rook
						else chss_brd_ptr->board[i][j][1] = 'K'; // King

					}




					// CPU
					else if (j == 6){
						chss_brd_ptr->board[i][j][0] = 'B';
						chss_brd_ptr->board[i][j][1] = 'P';
					}





					// Player
					else if (j == 1){
						chss_brd_ptr->board[i][j][0] = 'W';
						chss_brd_ptr->board[i][j][1] = 'P';
					}




					//empty pieces
					else{
						chss_brd_ptr->board[i][j][0] = '*';
						chss_brd_ptr->board[i][j][1] = '*';
					}
				}
			}




			
			for (i = 0; i < 6; i++){
				chss_brd_ptr->outOfBoard[0][i] = 0;
				chss_brd_ptr->outOfBoard[1][i] = 0;
			}
		



			
			chss_brd_ptr->LocationOfKing[0][0] = 4; // white x

			chss_brd_ptr->LocationOfKing[1][0] = 4; // black x
			//y cordinate 
			chss_brd_ptr->LocationOfKing[0][1] = 0; // white y


			chss_brd_ptr->LocationOfKing[1][1] = 7; // black y
			chss_brd_ptr->gameOver = false;

		}
		else crnt_command_bufferer = INVFMT;
	}


	// 01 == Print Board
	else if ((command_buffer[0] == '0') && (command_buffer[1] == '1')){	
		if (command_buffer[2] == '\n'){
			if (!chss_brd_ptr) crnt_command_bufferer = NOGAME;


			else crnt_command_bufferer = PRNTBRD;


		}
		else{
			crnt_command_bufferer = INVFMT;
		}		
	}



	// 02 == Player Move
	else if (command_buffer[0] == '0' &&  command_buffer[1] == '2'){
		if (!chss_brd_ptr){


			crnt_command_bufferer = NOGAME;


			return length;
		}

		// Comparing char by char for each character from the command being sent in by the viewer
		//ascii for a to h ... 97 and 104
		//ascii for 0 to 8 ... 48 and 56
		//current postion
		// -
		//position to move to 
		if ((command_buffer[2] == ' ') &&  ((command_buffer[3] == 'W') || (command_buffer[3] == 'B')) && ((command_buffer[4] == 'P') || (command_buffer[4] == 'R') || (command_buffer[4] == 'N') || (command_buffer[4] == 'B') || (command_buffer[4] == 'K') || (command_buffer[4] == 'Q'))
			&&  (command_buffer[5] >= 'a') &&  (command_buffer[5] <= 'h')  &&  (command_buffer[6] >= '0') &&  (command_buffer[6] <= '8')   
			&&  (command_buffer[7] == '-' ) 
			&&  (command_buffer[8] >= 'a') &&  (command_buffer[8] <= 'h') &&  (command_buffer[9] >= '0') &&  (command_buffer[9] <= '8')) {

			//check comm&& string length (has 02 WP included in length)
			if (length == 17 &&  (command_buffer[10] == 'x' || command_buffer[10] == 'y') &&  (command_buffer[11] == 'W' || command_buffer[11] <= 'B') &&  (command_buffer[4] == 'P' || command_buffer[4] == 'R' || command_buffer[4] == 'N' || command_buffer[4] == 'B' || command_buffer[4] == 'K' || command_buffer[4] == 'Q')	
				&&  (command_buffer[13] == 'x' || command_buffer[13] == 'y')
				&&  (command_buffer[11] == 'W' || command_buffer[11] <= 'B') &&  (command_buffer[4] == 'P' || command_buffer[4] == 'R' || command_buffer[4] == 'N' || command_buffer[4] == 'B' || command_buffer[4] == 'K' || command_buffer[4] == 'Q') &&  command_buffer[16] == '\n'){
				if (chss_brd_ptr->whitePlayers == chss_brd_ptr->whitePlayersTurn){
					



					//02 
					// move player
					if (chss_brd_ptr->gameOver){
						crnt_command_bufferer = NOGAME;
					}
					else{
						crnt_command_bufferer = move_player(chss_brd_ptr, command_buffer);
		
						if (crnt_command_bufferer == OK || crnt_command_bufferer == CHECK){
							chss_brd_ptr->whitePlayersTurn = !chss_brd_ptr->whitePlayersTurn;
						}
						if (crnt_command_bufferer == CHECK){
							chss_brd_ptr->cpuCheck = true;
						}

						if (crnt_command_bufferer == MATE){
							//04
							chss_brd_ptr->gameOver = true;
							printk(KERN_INFO "end game....\n");
						}
					}
				}
				else{
					crnt_command_bufferer = OOT;
				}
			}
			if (length == 11 &&  command_buffer[10] == '\n'){
				if (chss_brd_ptr->whitePlayers == chss_brd_ptr->whitePlayersTurn){ 
					if (chss_brd_ptr->gameOver){
						crnt_command_bufferer = NOGAME;
					}
					else{
						crnt_command_bufferer = move_player(chss_brd_ptr, command_buffer);
		

						if (crnt_command_bufferer == OK || crnt_command_bufferer == CHECK){
							chss_brd_ptr->whitePlayersTurn = !chss_brd_ptr->whitePlayersTurn;
						}
						if (crnt_command_bufferer == CHECK){
							chss_brd_ptr->cpuCheck = true;
						}
	
						if (crnt_command_bufferer == MATE){
							chss_brd_ptr->gameOver = true;
							printk(KERN_INFO "end game....\n");
						}
					}
				


				}
				else{
					crnt_command_bufferer = OOT;
				}
			}
			else if (length == 14 &&  (command_buffer[10] == 'x' || command_buffer[10] == 'y') &&  (command_buffer[11] == 'W' || command_buffer[11] <= 'B') &&  (command_buffer[4] == 'P' || command_buffer[4] == 'R' || command_buffer[4] == 'N' || command_buffer[4] == 'B' || command_buffer[4] == 'K' || command_buffer[4] == 'Q') 
			&&  command_buffer[13] == '\n'){
				if (chss_brd_ptr->whitePlayers == chss_brd_ptr->whitePlayersTurn){
					

					if (chss_brd_ptr->gameOver){
						crnt_command_bufferer = NOGAME;
					}
					else{
						crnt_command_bufferer = move_player(chss_brd_ptr, command_buffer);
		
						
						if (crnt_command_bufferer == OK || crnt_command_bufferer == CHECK){
							chss_brd_ptr->whitePlayersTurn = !chss_brd_ptr->whitePlayersTurn;
						}
						if (crnt_command_bufferer == CHECK){
							chss_brd_ptr->cpuCheck = true;
						}
		
						
						if (crnt_command_bufferer == MATE){
							
							chss_brd_ptr->gameOver = true;
							printk(KERN_INFO "end game....\n");
						}
					}
				
				}
				else{
					crnt_command_bufferer = OOT;
				}
			}
			else{
				crnt_command_bufferer = INVFMT;
			}
		}
		else{
			crnt_command_bufferer = INVFMT;
		}
		/**************************** valid format end *******************************/	
		
	}


	// 003 == CPU Move
	else if (command_buffer[0] == '0' &&  command_buffer[1] == '3'){
		if (command_buffer[2] == '\n'){
			
			if (chss_brd_ptr->whitePlayersTurn != chss_brd_ptr->whitePlayers){
				
				printk(KERN_INFO "CPU move....");
	
				if (chss_brd_ptr->gameOver){
					crnt_command_bufferer = NOGAME;
				}
				else{
					crnt_command_bufferer = move_cpu_piece(chss_brd_ptr);

					if (crnt_command_bufferer == OK  || crnt_command_bufferer == CHECK){
						chss_brd_ptr->whitePlayersTurn = !chss_brd_ptr->whitePlayersTurn;
					}
		
					
					if (crnt_command_bufferer == MATE){
						
						chss_brd_ptr->gameOver = true;


						printk(KERN_INFO "end game....\n");
					}
				}

			}
			else{
				crnt_command_bufferer = OOT;
			}
		}
		else{
			crnt_command_bufferer = INVFMT;
		}
	}
	// End Game
	else if (command_buffer[0] == '0' &&  command_buffer[1] == '4')
	{
		if (command_buffer[2] == '\n'){
			if (!chss_brd_ptr || chss_brd_ptr->gameOver == true) crnt_command_bufferer = NOGAME;
			else {
				chss_brd_ptr->gameOver = true;
				printk(KERN_INFO "end game....\n");
				crnt_command_bufferer = OK;
			}
		}
		else{
			crnt_command_bufferer = INVFMT;
		}
	}

	else{
		crnt_command_bufferer = UNKCMD;
	}
	return length;
}

// RELEASE
int chess_release(struct inode *pinode, struct file *pfile){
	printk(KERN_INFO "Device FILE closed...\n");
	kfree(chss_brd_ptr);
	return 0;
}
                                  

