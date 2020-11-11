/*
 This program will fork two child processes running the two programs generated in programming zero in parallel.
 Hint: You mant need to use fork(), exec() family, wait(), exit(), getpid() and etc ...
 
 Requirements:
 
 1) Exactly two child processes are created, one to run testspecial program and the other is to run testalphabet program;
 2) When a program starts to run, print a message to the output screen showing which process (with PID) is running which program, for example:
    "CHILD <16741> process is executing testalphabet program!"
    
 3) When a program is done, the process exits and at the same time, a message should be print to the output screen showing which  process (with PID) is done with the program, for example:
    "CHILD <16741> process has done with testalphabet program !"
    
 4) The messages should match the real execution orders, i.e. when the testspecial program starts/ends, the right message should be print out. So you need to figure out how to get the starting/ending time of each 
 process.
   
   
 The expected screen print out should be similar as follows:
 
 CHILD <16741> process is executing testalphabet program!
 CHILD <16742> process is executing testspecial program!
, -> 745668
. -> 798072
... ...

CHILD <16742> process has done with testspecial program !
a -> 2861232
b -> 494472
... ...

CHILD <16741> process has done with testalphabet program !
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
	
	pid_t alphaPID, specialPID;	//setting up Process ID variables for testalphabet and testspecial
	if(!(alphaPID = fork()))	//if fork() runs without error, then execute testalphabet child process
	{
		printf("CHILD <PID: %d> process is executing testalphabet program!\n", (int)getpid());
		char *args[] = {"./testalphabet", NULL}; 
		execv(args[0], args);
	}
	else if(!(specialPID = fork())) //if fork() runs without error, then execute testspecial child process
	{
		printf("CHILD <PID: %d> process is executing testspecial program!\n", (int)getpid());
		char *args[] = {"./testspecial", NULL}; 
		execv(args[0], args);
	}
	else
	{
		printf("CHILD <PID: %d> process has done with testspecial program!\n", (int)(wait(&specialPID)));	//waits for the child process in specialPID to finish
		printf("CHILD <PID: %d> process has done with testalphabet program!\n", (int)(wait(&alphaPID)));	//waits for the child process in alphaPID to finish
	}

	return 0;
	
}
