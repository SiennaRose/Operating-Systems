/*
 * alphabetcountmulthreads.c - this file implements the alphabetcountmulthreads function.
 */

#include <stdio.h> 
#include <string.h>
#include <pthread.h> 
#include <stdlib.h>
#include <dirent.h> 
#include "count.h"

/**
  The alphabetcountmulthreads function counts the frequency of each alphabet letter (a-z, case insensitive) in all the .txt files under
  directory of the given path and write the results to a file named as filetowrite. Different with programming assignment#0, you need to implement the program using mutithreading.
  
  Input: 
      path - a pointer to a char string [a character array] specifying the path of the directory; and
      filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
      alphabetfreq - a pointer to a long array storing the frequency of each alphabet letter from a - z, which should be already up-to-date;
      num_threads - number of the threads running in parallel to process the files
      
       
  Output: a new file named as filetowrite with the frequency of each alphabet letter written in
  
  Requirements:
1)	Multiple threads are expected to run in parallel to share the workload, i.e. suppose 3 threads to process 30 files, then each thread should process 10 files;
2)	When a thread is created, a message should be print out showing which files this thread will process, for example:
Thread id = 274237184 starts processing files with index from 0 to 10!
3)	When a file is being processed, a message should be print out showing which thread (thread_id = xxx) is processing this file, for example:
Thread id = 265844480 is processing file input_11.txt
4)	When a thread is done with its workload, a message should be print out showing which files this thread has done with work, for example:
      Thread id = 274237184 is done !
5)	The array: long alphabetfreq[ ] should always be up-to-date, i.e. it always has the result of all the threads counted so far.  [You may need to use mutexes to protect this critical region.]


You should have the screen printing should be similar as follows:

 Thread id = 274237184 starts processing files with index from 0 to 10!
 Thread id = 265844480 starts processing files with index from 11 to 22!
 Thread id = 257451776 starts processing files with index from 23 to 31!

 Thread id = 265844480 is processing file input_11.txt
 Thread id = 257451776 is processing file input_22.txt
 Thread id = 274237184 is processing file input_00.txt
  ??

 Thread id = 274237184 is done !
 Thread id = 265844480 is done !
 Thread id = 257451776 is done !

 The results are counted as follows:
 a -> 2861232
 b -> 494472
 c -> 747252
 ? ?
*/

struct file_bounds_struct //data passed to each thread
{
	int start;	//the start of the file group the thread works on in array
	int end;	//the end of file group the thread works on
	char *path;  //path to directory where files are from 
};

long temp_alphabetfreq[ALPHABETSIZE] = {0}; 
char **array; //holds all the files being read in from the directory specified by path

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //mutex variable initialized

void* read_alphabetfreq_thread(void* arg)
{
	//go through each file in the file bounds given and read the freq of each alphabet char
	struct file_bounds_struct *arg_struct = (struct file_bounds_struct*) arg; 
	printf("Thread id = %u starts processing files with index from %d to %d!\n", pthread_self(), arg_struct->start, arg_struct->end);

	FILE *file; 
	char full_filePath[30]; 
	int character; 	//used to hold character read in from file	

	//open files
	for(int i = arg_struct->start; i <= arg_struct->end; i++)
	{
		printf("Thread id = %u is processing file %s\n", pthread_self(), array[i]); 
		
		//create working pathname 
		strcpy(full_filePath, arg_struct->path);
		strcat(full_filePath, "/");
		strcat(full_filePath, array[i]);
		
		file = fopen(full_filePath, "r");
		if(file == NULL)
			perror("Error opening file");
		else
		{
			do
			{
				pthread_mutex_lock(&mutex); //start critical section
				character = getc(file); 
				
				if(character <= 'z' && character >= 'A')
				{
					if(character >= 'a')
					{
						temp_alphabetfreq[character-71]++; //upper case letter was found
					}
					else
					{
						temp_alphabetfreq[character-65]++; //lower case letter was found
					}
				}
				pthread_mutex_unlock(&mutex);	//end critical section
			}
			while(character != EOF);
		}
		fclose(file);
	}
	
	printf("Thread id = %u is done!\n", pthread_self());
	pthread_exit(0); 	
}

void alphabetcountmulthreads(char *path, char *filetowrite, long alphabetfreq[], int num_threads)
{
	//find out how many files there are total to determine how many files to send to each thread	
	int fileCount = 0; 
	DIR *dir; 
	struct dirent *entry; 

	char *dot = "\0"; 
	char filePath[30]; 
	
	//go through directory and read every .txt file path into an array		
	int arraySize = 35; 
	array = malloc(arraySize * sizeof(char*));
	
	dir = opendir(path); 
	if(dir == NULL)
	{
		perror("Unable to read dir"); 
	}	
	else
	{
		//see if there are any files in the directory
		while((entry = readdir(dir)))
		{
			//check if files is a .txt file
			dot = strrchr(entry->d_name, '.');
			if(dot)
			{
				if(strcmp(dot, ".txt") == 0)
				{
					//save .txt file name in array
					strcpy(filePath, entry->d_name);
					array[fileCount] = (char *)malloc(strlen(filePath) + 1);
					strcpy(array[fileCount], filePath); 
					fileCount++; 
				}
			}
		}
	}
	closedir(dir);

	struct file_bounds_struct args[num_threads];	//struct that holds the arguments that will be passed to each of the threads
	int files_per_thread = fileCount / num_threads;

	//make the threads
	pthread_t tids[num_threads]; 
	for(int i = 0; i < num_threads; i++)
	{
		args[i].start = i * files_per_thread;

		//when there are no more threads, give the last thread the remaining files to work on
		if(i == (num_threads - 1))	
			args[i].end = fileCount - 1;
		else
			args[i].end = args[i].start + files_per_thread - 1; 	

		//save the path in the files_bound_struct
		args[i].path = path; 

		//create a pthread_attr for the file_bounds_struct meant to hold the start and end bounds of the files to be read by the thread
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tids[i], &attr, read_alphabetfreq_thread, &args[i]); 

	}

	//join all threads
	for(int i = 0; i < num_threads; i++)
	{
		pthread_join(tids[i], NULL);		
	}

	free(array); //free memory being used to hold
	
	for(int i = 0; i < ALPHABETSIZE; i++)
	{
		alphabetfreq[i] = temp_alphabetfreq[i]; 
	}

	//write alphabetfreq[] to filetowrite
	FILE *file; 
	file = fopen(filetowrite, "w");
	if(file == NULL)
	{
		perror("Cannot open file");
	}
	else
	{
		for(int i = 0; i < ALPHABETSIZE; i++)
		{
			if(i < 26) //first 26 indicies are upper case letters
			{
				fprintf(file, "%c -> %d\n", (char)(i+65), alphabetfreq[i]); 
			}
			else //last 26 are lower case letters
			{
				fprintf(file, "%c -> %d\n", (char)(i+71), alphabetfreq[i]); 
			}
		}
	}
	fclose(file); 
}
