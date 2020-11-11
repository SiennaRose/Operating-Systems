/*
 * specialcharcount.c - this file implements the specialcharcount function.
 */


#include <stdio.h> 
#include <dirent.h>
#include <string.h>
#include "count.h"

void specialcharcount(char *path, char *filetowrite, long charfreq[])
{
	DIR *dir; 		//used to open and close the directory
	struct dirent *entry;	//pointer to files in the directory
	FILE *file;		//used to open and close files
	char filePath[30]; 	//string used to hold filename
	char *dot = "\0";	//char pointer used to check file is .txt
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
			//check if file is a .txt file
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
						do 	//go through every character in the file
						{
							character = getc(file); 
							
							//check if the character is a special character	
							if(character == ',')
								charfreq[0]++;
							if(character == '.')
								charfreq[1]++;
							if(character == ':')
								charfreq[2]++;
							if(character == ';')
								charfreq[3]++;
							if(character == '!')
								charfreq[4]++; 
						}
						while(character != EOF); 
					}
					fclose(file); 
				}
			}
		}
	}
	closedir(dir); 

	//write alphabetfrew[] to filetowrite
	file = fopen(filetowrite, "w");
	if(file == NULL)	
	{
		perror("Cannot open file");
	}
	else
	{
		int i;
		for(i = 0; i < SPECIALCHARSIZE; i++)	
		{
			switch(i)
			{
				case 0: 
					fprintf(file, ", -> %d\n", charfreq[i]);
					break;
				case 1:
					fprintf(file, ". -> %d\n", charfreq[i]);
					break;
				case 2:
					fprintf(file, ": -> %d\n", charfreq[i]);
					break;
				case 3: 
					fprintf(file, "; -> %d\n", charfreq[i]);
					break;
				case 4: 
					fprintf(file, "! -> %d\n", charfreq[i]);
					break;
			}
		}
	}
	fclose(file);
}
/**
  The specialcharcount function counts the frequency of the following 5 special characters:
  ','   '.'   ':'    ';'    '!'
   
  in all the .txt files under directory of the given path and write the results to a file named as filetowrite.
  
  Input: 
  
      path - a pointer to a char string [a character array] specifying the path of the directory; and
      filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
      alphabetfreq - a pointer to a long array storing the frequency of the above 5 special characters
      
      charfreq[0]: the frequency of ','
      charfreq[1]: the frequency of '.'
      charfreq[2]: the frequency of ':'
      charfreq[3]: the frequency of ';'
      charfreq[4]: the frequency of '!'

  
  Output: a new file named as filetowrite with the frequency of the above special characters written in
  
  Steps recommended to finish the function:
  1) Find all the files ending with .txt and store in filelist.
  2) Read all files in the filelist one by one and count the frequency of each alphabet letter only (a - z). The array 
  long alphabetfreq[] always has the up-to-date frequencies of alphabet letters counted so far. If the letter is upper case, convert
  it to lower case first. 
  3) Write the result in the output file: filetowrite in following format: 
  
     character -> frequency
     
     example:
     , -> 20
     . -> 11
     : -> 20
     ; -> 11
     ! -> 12     
     
  Assumption:  
  1) You can assume there is no sub-directory under the given path so you don't have to search the files 
  recursively.    
  2) Only .txt files are counted and other types of files should be ignored.
  
*/

