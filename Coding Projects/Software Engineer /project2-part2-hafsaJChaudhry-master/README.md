# project2-part2-hafsaJChaudhry
project2-part2-hafsaJChaudhry created by GitHub Classroom

Part 2: Chess Game (40% of Project 2 Grade)
kernel module used to play against CPU in chess!

implementations:
1)virtual character device (with a corresponding entry in /dev that the user can open a file descriptor to)
2)interact with the user using server-viewer and file read and write operations

Module will work by: 
1)user opens a file descriptor to the module's /dev entry (which shall be named /dev/chess)
2)writes commands to that file descriptor with the write() system call
3)reads the responses to those commands with the read() system call

Module named chess.ko once it is compiled.


**********************INSTRUCTIONS***************************
make
sudo insmod chess.ko

follow TA instructions to go to chess-viewer

play on chess-viewer on native desktop

then play game. enjoy
*************************************************************
