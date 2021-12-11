#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/TicTacToe"
#define X "x"
#define O "o"

int8_t write_buf[1024];
int8_t read_buf[1024];


/*
void currentBoard(char pos[][3]){
	printf("\n\n"); 
      	printf("\t\t\t  %c | %c  | %c  \n", pos[0][0],pos[0][1],pos[0][2]); 
    	printf("\t\t\t--------------\n"); 
    	printf("\t\t\t  %c | %c  | %c  \n", pos[1][0],pos[1][1],pos[1][2]); 
    	printf("\t\t\t--------------\n"); 
    	printf("\t\t\t  %c | %c  | %c  \n\n", pos[2][0],pos[2][1],pos[2][2]);
	return; 
}


void newGame(char pos[][3]){
/*
		printf("\t\t    * |  *  | *  \n"); 
    		printf("\t\t  ----------------\n"); 
    		printf("\t\t    * |  *  | *  \n"); 
   		printf("\t\t  ----------------\n"); 
    		printf("\t\t    * |  *  | *  \n\n");
*/
/*
	for(int in=0;in<3;in++){
		for(int inn=0;inn<3;inn++){
			board[in][inn] = '*';
		}
	}
	return;
}
bool validInput(inp){
	if(inp == 00 || inp == 01 || inp == 02 || inp == 10 || inp == 11 || inp == 12 || inp == 20 || inp == 21 || inp == 22){
		return true;
	}
	else{
		return false;
	}
}

bool already(char pos[][3]){
	for(int in=0; in<3; in++){
		if((pos[0][in] == pos[1][in])and(pos[1][in]==pos[2][in])and(pos[0][in] != '*'))
			return true;
		if((pos[in][0] == pos[in][1])and(pos[in][1] == pos[in][2])and(pos[in][0] != '*'))
			return true;
		if((pos[0][0] == pos[1][1])and(pos[1][1]==pos[2][2])and(pos[0][0] != '*'))
			return true;
		if((pos[0][2]==pos[1][1])and(pos[1][1]==pos[2][0])and(pos[0][2] != '*'))
			return true;
	}
	return false;
}*/

int main(){
	int fd; 
	char option; 
	printf("Welcome to the character device driver game console...\n");

	/* open file with path to dev */
	fd = open(DEVICE, O_RDWR);
	if(fd < 0){
		printf("Cannot open device file...\n");
		return 0;
	}
	while(1){
		printf("\t\t\t  Tic-Tac-Toe\n"); 
    		printf("\t\tTo play, choose a cell from below\n\n");   
    		printf("\t\t    (0,0) |  (1,0)  | (2,0)  \n"); 
    		printf("\t\t  --------------------------\n"); 
    		printf("\t\t    (0,1) |  (1,1)  | (2,1)  \n"); 
   		printf("\t\t  --------------------------\n"); 
    		printf("\t\t    (0,2) |  (1,2)  | (2,2)  \n\n");   

		printf("******please enter your option*****\n");
		printf("             0. To Start a Game              \n");
		printf("             1. To Return Current Board      \n");
		printf("             2. Make a Player Move           \n");
		printf("             3. Let Computer Make a Move     \n");
		printf("             4. To Exit Game                 \n");

		scanf(" %c",  &option);
		printf(" you chose = %c\n", option);
		
		switch(option){
			case '0':
				printf("Type 'x' to be player X and 'o' to be player O:\n");
				scanf(" %[^\t\n]s", write_buf);
				write(fd, write_buf, strlen(write_buf)+1);
				if(write_buf == X){
					//computer = O;
					printf("ok\n");
				}
				if(write_buf == O){
					//computer = X;
					printf("ok\n");
				}
				else{
					printf("Invalid option. Try again:\n");
					scanf(" %[^\t\n]s", write_buf);
				}
				//printf("You chose to be player = %s\n", write_buf);

				break;
			case '1':
				//currentBoard();
				
				printf("Data is reading...\n");
				read(fd,read_buf, 1024);
				printf("Done...\n\n");
				//printf("Data = %s\n\n", read_buf);
				break;
			case '2':
				printf("Option 02...\n");
				break;
			case '3':
				printf("Option 03...\n");
				break;
			case '4':
				close(fd);
				exit(1);
				break;
			default:
				printf("Enter valid option = %c\n", option);
				break;
		}
	}
	printf("Goodbye!");
	close(fd);
}
