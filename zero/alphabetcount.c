/*
 * alphabetcount.c - this file implements the alphabetlettercount function.
 */

#include <stdio.h>
#include <dirent.h> 
#include <string.h>
#include "count.h"

/**
  The alphabetlettercount function counts the frequency of each alphabet letter (A-Z a-z, case sensitive) in all the .txt files under
  directory of the given path and write the results to a file named as filetowrite.
  
  Input: 
      path - a pointer to a char string [a character array] specifying the path of the directory; and
      filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
      alphabetfreq - a pointer to a long array storing the frequency of each alphabet letter from A - Z a - z:
      alphabetfreq[0]: the frequency of 'A'
      alphabetfreq[1]: the frequency of 'B'
         ... ...
      alphabetfreq[25]:the frequency of 'Z'
      alphabetfreq[26]:the frequency of 'a'
         ...  ...
      alphabetfreq[51]:the frequency of 'z'
  
  Output: a new file named as filetowrite with the frequency of each alphabet letter written in
  
  Steps recommended to finish the function:
  1) Find all the files ending with .txt and store in filelist.
  2) Read all files in the filelist one by one and count the frequency of each alphabet letter only (A-Z a - z). The array 
  long alphabetfreq[] always has the up-to-date frequencies of alphabet letters counted so far. 
  3) Write the result in the output file: filetowrite in following format: 
  
     letter -> frequency
     
     example:
     A -> 200
     B -> 101
     ... ...
     
  Assumption:  
  1) You can assume there is no sub-directory under the given path so you don't have to search the files 
  recursively.    
  2) Only .txt files are counted and other types of files should be ignored.
  
*/
void alphabetlettercount(char *path, char *filetowrite, long alphabetfreq[])
{
	DIR *dir;		//directory pointer
	struct dirent *entry;	//pointer to files in the directory
	FILE *file;		//file pointer
	char filePath[30];	//string used to hold filename
	char *dot = "\0"; 	//char pointer used to check file is .txt
	int character; 		//int used to hold character read in from file
	
	//open directory specified by path
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
			//Check if file is a .txt file
			dot = strrchr(entry->d_name, '.');	
			if(dot)
			{
				if(strcmp(dot, ".txt") == 0)
				{
					//combine path and filename to create filePath
					strcpy(filePath, path);
					strcat(filePath, "/");
					strcat(filePath, entry->d_name);
					
					//open file
					file = fopen(filePath, "r");
					if(file == NULL)
					{
						perror("Error opening file");
					}
					else
					{
						do	//go through every character in the file
						{
							character = getc(file); 
							
							//check if character is a letter	
							if(character <= 'z' && character >= 'A')
							{
								if(character >= 'a') //check if letter is lower case
								{
									alphabetfreq[character-71]++; 	
								}	
								else //upper case letters minus 65 to get to start at 0 index
								{
									alphabetfreq[character-65]++;	
								}
							}	
						}
						while(character != EOF);
					}	
					fclose(file);
				}
			}
		}

	}	
	closedir(dir);

	//write alpahbetfreq[] to filetowrite
	file = fopen(filetowrite, "w"); 
	if(file == NULL)
	{
		perror("Cannot open file");
	}
	else
	{
		int i;
		for(i = 0; i < ALPHABETSIZE; i++)
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
