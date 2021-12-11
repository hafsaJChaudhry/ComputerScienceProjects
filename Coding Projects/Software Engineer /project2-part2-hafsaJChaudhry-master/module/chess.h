#pragma once
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/random.h>

#define MODULE_NAME "chess"


#define INVFMT -2
#define UNKCMD -1
#define OK	0
#define CHECK	1
#define MATE	2
#define ILLMOVE	3
#define OOT 	4
#define NOGAME 	5
#define PRNTBRD 6
#define WHITE	0
#define BLACK	1
#define PAWN	0
#define ROOK	1
#define	KNIGHT	2
#define BISHOP	3
#define QUEEN	4
#define KING	5

typedef struct chess_board
{
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


bool CheckForCheck(int x, int y, bool isWhite, struct chess_board *c_ptr);
bool CheckForMate(int x, int y, bool isWhite, struct chess_board *c_ptr);

int chess_open(struct inode *pinode, struct file *pfile);
ssize_t chess_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
ssize_t chess_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);
int chess_release(struct inode *pinode, struct file *pfile);


