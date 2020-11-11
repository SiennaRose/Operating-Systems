Author: Sienna Rose Sacramento 
ID: cssc2359
Class: CS570 Operating Systems
Professor: Dr. Zheng Li
Assignment #0

## Extra Libraries Used

dirent.h was used to open and close directory specified by path and read the next file in the directory

string.h was used to combine the path string of the directory and filename in order to open and close files
	

#alphabetcount.c

The alphabetcount function counts the frequency of each alphabet letter (A-Z, a-z, case sensitive) in all the .txt files under the directory of the given path and write the results to a file named as filetowrite.

Input: 
	path - a pointer to a char string [a character array] specifying the path of the directory; and 
	filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
	alphabetfreq - a pointer to a long array storing the frequency of each alphabet letter from A-Z a-z:
		alphabetfreq[0]: the frequency of 'A'
		alphabetfreq[1]: the frequency of 'B'
			... ...
		alphabetfreq[25]: the frequency of 'Z'
		alphabetfreq[26]: the frequency of 'a'
			... ...
		alphabetfreq[51]: the frequency of 'z'
	
Output: a new file names as filetowrite with the frequency of each alphabet letter written in 



#specialcharcount.c	

The specialcharcount function counts the frequency of the following 5 special characters: ',' '.' ':' ';' '!'
in all the .txt files under directory of the given path and write the results to a file names as filetowrite.

Input: 
	path - a pointer to a char string [a character array] specifying the path of the directory; and 
	filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
	charfreq - a pointer to a long array storing the frequency of the above 5 special characters
		charfreq[0]: the frequency of ','
		charfreq[1]: the frequency of '.'
		charfreq[2]: the frequency of ':'
		charfreq[3]: the frequency of ';'
		charfreq[4]: the frequency of '!'

Output: a new file names as filetowrite with the frequency of the above special characters written in
