Author: Sienna Rose Sacramento 
ID: cssc2359
Class: CS570 Operating Systems
Professor: Dr. Zheng Li
Assignment #1

## Extra Libraries Used

sys/wait.h was used to allow a parent process to wait for the child process to finish executing. 

sys/types.h is necessary to create the pid_t variable.

unistd.h is necessary to use the fork(), execv(), and getpid() functions	

#testalphabet - Executible file of the alphabetcount.c file in assignment #0

#testspecial - Executible file of the specialcharcount.c file in assignment #0

#mulproc.c

This program forks two child processes and executes them using fork(), execv(), wait(), and getpid(). 

When the child process first starts it prints a message to the console:
	"CHILD <PID: ####> process is executing ______ program!"

When the child process ends it prints another message to the console:
	"CHILD <PID: ####> process has done with ______ program!"
	


