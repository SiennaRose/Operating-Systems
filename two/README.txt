Author: Sienna Rose Sacramento 
ID: cssc2359
Class: CS570 Operating Systems
Professor: Dr. Zheng Li
Assignment #2

## Extra Libraries Used

string.h is necessary for using strcpy(), strcat(), etc.

pthread.h is necessary for using and creating threads

dirent.h is necessary for opening directories 

stdlib.h is necessary for using malloc(), free(), etc.

## alphabetcountmulthreads.c 

The alphabetmulthreads function counts the frequency of each letter (a-z, case insensitive) in all the .txt files under 
directory of the given path and write the results to a file names as filetowrite. Different with programming 
assignment#0, you need to implement the program using multithreading. 

Input:
	path - a pointer to a char string [a character array] specifying the path of the directory; and
	filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
	alphabetfreq - a pointer to a long array storing the frequency of each alphabet letter from a - z, which should be already up-to-date;
	num_threads - number of the threads running in parallel to process the files

Output: a new file names as filetowrite with the frequency of each alphabet letter written in

Requirements: 
1) Multiple threads are expected to run in parallel to share the workload, i.e. suppose 3 threads to process 30 files, then each thread should process 10 files;
2) When a thread is created, a message should be print out showing which files this thread will process, for example: 
	Thread id = 274237184 starts processing files with index from 0 to 10!
3) When a file is being processed, a messafe should be print out showing which thread (thread_id = xx) is processing this file, for example: 
	Thread id = 265844480 is processing file input_11.txt
4) When a thread is done with its workload, a messafe should be print out showing which files this thread has done with work, for example:
	Thread id = 274237184 is done!
5) The array: long alphabetfrq[] should always be up-to-date, i.e. it always has the result of all the threads counted so far. [You may have to use mutexes to protect this critical region.]

