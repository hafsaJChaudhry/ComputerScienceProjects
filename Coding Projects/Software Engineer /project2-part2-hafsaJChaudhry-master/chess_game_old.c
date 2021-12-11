#include "chess.h"
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/random.h>

#define DEVICE "/dev/Chess"
#define BUF_SIZE PAGE_SIZE
#define INVFMT -2
#define UNKCMD -1
#define OK		0
#define CHECK   1
#define MATE	2
#define ILL 		3
#define OOT 	4
#define NOG	5
#define PRN	6

#define mem_size 1024
uint8_t *kernel_buffer;
//int8_t write_buf[1024];
//int8_t read_buf[1024];

/** The Board Structure Itself **/
typedef struct chess_board{
	// True if player is white
	bool playerWB;
	bool whiteTurn;
	char board[8][8][2];
	char graveyardW[8];
	char graveyardB[8];
	bool gameOver;
}chess_board;

struct chess_board *c_ptr;
uint8_t *curBuff;

/**
//new Game
void New_Game(bool plyrWB){
	int i, j;

	printk(KERN_INFO "In fxtn New_Game...");

	//true if player is white
	c_ptr->playerWB = plyrWB;

	//white always starts
	c_ptr->whiteTurn = true;

	//initialize board
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			//player pieces
			if(j == 0){
				c_ptr->board[i][j][0] = 'W';

				

				if(i==0 || i ==7) = 'P';
				else if(i==1 || i ==6) = 'R';
				else if(i==2 || i ==5) = 'Q';
				else if(i==3) c_ptr->board;
				

				else c_ptr->board[i][j][1] = 'K';
			}
			//player pawns
			else if(j==1){
				c_ptr->board[i][j][0] = 'W';
				c_ptr->board[i][j][1] = 'P';
			}
			//cpu pawns
			else if(j==0){
				c_ptr->board[i][j][0] = 'B';
				c_ptr->board[i][j][1] = 'P';
			}
			//CPU pieces
			else if(j == 7){
				c_ptr->board[i][j][0] = 'B';
				

				if(i==0 || i ==7) = 'P';
				else if(i==1 || i ==6) = 'R';
				else if(i==2 || i ==5) = 'Q';
				else if(i==3) c_ptr->board;
				

				else c_ptr->board[i][j][1] = 'K';
			}
			else{
				c_ptr->board[i][j][0] = '*';
				c_ptr->board[i][j][1] = '*';
			}	
		}
	}
	//creates empty graveYard for each 
	for(i = 0; i < 8; i++){
		c_ptr->graveyardW[i] = '*';
		c_ptr->graveyardB[i] = '*';
	}
	//sets the winner to *
	c_ptr->gameOver = false;

	//sets the current buffer
	//c_ptr->curBuff = OK;
}
**/
/**
//02 Player Move
static void Player_Move(void){
	printk(KERN_INFO "[" MODULE_NAME "] in Player_Move\n");
}

//03 CPU Move
static void CPU_Move(void){
	printk(KERN_INFO "[" MODULE_NAME "] in CPU_Move\n");
}

//04 End Game
static void End_Game(void){
	printk(KERN_INFO "[" MODULE_NAME "] in End_Game\n");
}**/
/**********************read and write**************************/
//in open return 0 if successful
int chess_open(struct inode *pinode, struct file *pfile){
	/* Creating Physical Memory */
	if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0){
		printk(KERN_INFO "Cannot allocate the memory to the kernel..\n");
		return -1;
	}
	printk(KERN_INFO "Device File opened...\n");
	return 0;
}

//in read return how many bytes you read, initially return 0
//for chess game, read is when you are sending it to the viewer
ssize_t chess_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
	//int i, j, k
	printk(KERN_INFO "in fxtn read...\n");

	//how you send messages back..printk prints out in kernel log and copy_to_user prints out to user
	/**
	if(curBuff == INVFMT){
		printk(KERN_INFO "move : INVFMT\n");
		copy_to_user(buffer, "INVFMT\n", 7);
		length = 7;
	}
	else if(curBuff == UNKCMD){
		printk(KERN_INFO "move : UNKCMD\n");
		copy_to_user(buffer, "UNKCMD\n", 7);
		length = 7;
	}
	else if(curBuff == OK){
		printk(KERN_INFO "move : OK\n");
		copy_to_user(buffer, "OK\n", 3);
		length = 3;
	}
	else if(curBuff == CHECK){
		printk(KERN_INFO "move : CHECK\n");
		copy_to_user(buffer, "CHECK\n", 6);
		length = 6;
	}
	else if(curBuff == MATE){
		printk(KERN_INFO "move : MATE\n");
		copy_to_user(buffer, "MATE\n", 5);
		length = 5;
	}
	else if(curBuff == ILL){
		printk(KERN_INFO "move : ILLMOVE....\n");
		copy_to_user(buffer, "ILLMOVE\n", 8);
		length = 8;
	}
	else if(curBuff == OOT){
		printk(KERN_INFO "move : OOT...\n");
		copy_to_user(buffer, "OOT\n", 4);
		length = 4;
	}
	else if(curBuff == NOG){
		printk(KERN_INFO "move : NOGAME...\n");
		copy_to_user(buffer, "NOGAME\n", 7);
		length = 7;
	}
	else if(curBuff == PRN){
		printk(KERN_INFO "Printing Board...\n");
		char prntBoard[129];
		k = 0;
		for(j=0;j<8;j++){
			for(i=0;i<8;i++){
				prntBoard[k] = c_ptr->board[i][j][0];
				k++;
				prntBoard[k] = c_ptr->board[i][j][1];
				k++;
			}		
		}
		prntBoard[128] = '\n';
		copy_to_user(buffer, prntBoard, 129);
		length = 129;
	}
	else{
		printk(KERN_INFO "ELSE...\n");
		return -1;
	}

	return length;
	**/
}

//in write you have to tell them how many bytes you are writing, initially return length
//for chess game, write is when you are getting stuff from the viewer
ssize_t chess_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
	//char buff[length];
	//copy_from_user(buff, buffer, length);
	printk(KERN_INFO "[" MODULE_NAME "] in fxtn write...\n");

	
	//new game
	if(buff[0] == '0' && buff[1] == '0'){
		if(buff[2] == ' ' && buff[3] == "W" && buff[4] == '\n') New_Game(true);
		else if(buff[2] == ' ' && buff[3] == "B" && buff[4] == 'n')New_Game(true);
		else curBuff = INVFMT;
	} 
	//print board
	else if(buff[0] == '0' && buff[1] == '1'){
		if(buff[2] == '\n'){
			if(!c_ptr || c_ptr->gameOver == true) curBuff = NOG;
			else curBuff = PRN;
		}
		else curBuff = INVFMT;	
	}
	//player move
	else if(buff[0] == '0' && buff[1] == '2'){
		if(!c_ptr || c_ptr->gameOver == true){
			printk(KERN_INFO "Game over test...\n");
		}
	}
	
}

//in release return 0 bc no error
int chess_release(struct inode *pinode, struct file *pfile){
	//kfree(kernel_buffer);
	printk(KERN_INFO "Device FILE closed...\n");
	return 0;
}
/*************************end**********************************/
