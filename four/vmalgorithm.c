/*
 *  Implementation of FIFO and LRU page replacement algorithm
 *  Please add appropriate level of comments in this file 
 */

#include "vmalgorithm.h"


/* Generate an access pattern
 * Example: 3, 2, 2, 1, 1  indicates the page 3, 2, 2, 1, 1 in order
 */
void generateAccessPattern()
{
   int i;
   srand(time(0));
   accessPattern = (int *)malloc( sizeof(int) * AccessPatternLength);   
   printf("The randomized Access Pattern: ");
   for(i=0; i< AccessPatternLength; i++)
   {
		     accessPattern[i] = rand() % ReferenceSZ;
        printf("%d ", accessPattern[i]);       
   }
   printf("\n");
}

/*
 * Initialize the parameters of simulated memory
 */
void initializePageFrame()
{
   int i;
   memory.PageFrameList = (int *)malloc( sizeof(int)* FrameNR );    // dynamic allocated FrameNR frames to be used in memory
   memory.nextToReplaced =0;          // indicate the new frame to be replaced as 0
   for(i=0; i< FrameNR; i++)
   {
			memory.PageFrameList[i] = -1;  // initialization to -1 indicating all frames are unused 
   }

}

// Print the pages loaded in memory
void printPageFrame()
{
   int i;
   for(i=0; i< FrameNR; i++)
   {
			printf("%2d ",memory.PageFrameList[i]);       
   }
   printf("\n");
}


/*
 *  Print the access pattern in order
 */
void printAccessPattern()
{
   int i;
   printf("The Same Access Pattern: ");
   for(i=0; i< AccessPatternLength; i++)
   {
        printf("%d ", accessPattern[i]);       
   }
   printf("\n");

}


/*
 * Return: number of the page fault using FIFO page replacement algorithm
 */
int FIFO()
{
	int pageFaults = 0;
	int currSize = 0; 
	
	//loop through all the ints inside of the accessPattern
	int i; 
	for(i = 0; i < AccessPatternLength; i++)
	{
		int j;	//counter variable used to traverse through PageFrameList

		//check if PageFrameList is full
		//if not full, then add the new Page
		if(currSize < FrameNR)
		{
			int indexOfPresentPage = -1; 
			
			//if there is still room in PageFrameList, then see if that page is already in PageFrameList			
			for(j = 0; j < currSize; j++)
			{
				if(memory.PageFrameList[j] == accessPattern[i])	//if page already exists, remember its index in PageFrameList
				{
					indexOfPresentPage = j; 			
				}
			}

			if(indexOfPresentPage == -1) //if the most recent page does not exist in PageFrameList, then add it
			{
				memory.PageFrameList[currSize] = accessPattern[i];
				currSize++; 

				pageFaults++; 
			}
		}
		else	//PageFrameList is full so we have to replace a page or the page already exists
		{
			int indexOfPresentPage = -1; 
			
			//if there is still room in PageFrameList, then see if that page is already in PageFrameList			
			for(j = 0; j < currSize; j++)
			{
				if(memory.PageFrameList[j] == accessPattern[i])	//if page already exists, remember its index in PageFrameList
				{
					indexOfPresentPage = j; 			
				}
			}
			
			if(indexOfPresentPage == -1)	//first in element in PageFrameList must be replaced by new Page
			{
				memory.PageFrameList[memory.nextToReplaced] = accessPattern[i]; 	//elemnt in the index specified by nextToReplaced is replaced
				if(memory.nextToReplaced == FrameNR - 1)	//update nextToReplaced
					memory.nextToReplaced = 0; 
				else
					memory.nextToReplaced++; 
				pageFaults++; 
			}
		}
		printPageFrame(); 
	}

	return pageFaults;    
}



/*
 * Return: number of the page fault using LRU page replacement algorithm
 */

int LRU()
{
	int pageFaults = 0;	//initalize page fault; this is the value that will be returned  
	int currSize = 0; 	//initialize the currSize, holds the number of pages currently in PageFrameList

	//dynamically-allocated array representing the least recently used page in the page frame list
	int *lruPage = (int *)malloc(sizeof(int) * FrameNR); 

	//initalize all indexes inside lruPage to 0 since PageFrameList is empty and no pages have been recently used
	int i; 
	for(i = 0; i < FrameNR; i++)
	{
		lruPage[i] = 0; 
	}
	
	//loop through all the ints inside of the accessPattern
	for(i = 0; i < AccessPatternLength; i++)
	{
		int j;	//counter variable used to traverse through PageFrameList
		int recentlyUsedPage; //saves the most recently used Page's index in PageFrameList

		//check if PageFrameList is full
		//if not full, then add the new Page or update lruPage array
		if(currSize < FrameNR)
		{
			recentlyUsedPage = -1;
			
			//if there is still room in PageFrameList, then see if that page is already in PageFrameList			
			for(j = 0; j < currSize; j++)
			{
				if(memory.PageFrameList[j] == accessPattern[i])	//if page already exists, remember its index in PageFrameList
				{
					recentlyUsedPage = j; 			
				}
			}

			if(recentlyUsedPage == -1) //if the most recent page does not exist in PageFrameList, then add it
			{
				memory.PageFrameList[currSize] = accessPattern[i];
				lruPage[currSize] = i; 
				currSize++; 

				pageFaults++; 
			}
			else
			{
				lruPage[currSize] = i; 
			}

		}
		else	//PageFrameList is full and the least recently used page might need to be removed
		{
			recentlyUsedPage = -1; 			

			for(j = 0; j < currSize; j++)	
			{
				if(memory.PageFrameList[j] == accessPattern[i])	//if page already exists, then no need to replace pages
					recentlyUsedPage = j; 
			}

			int oldestPage = AccessPatternLength + 1; //used to hold the earliest seen page inside of PageFrameList
			int indexOfOldestPage = -1; 

			if(recentlyUsedPage == -1)	//page was not in PAgeFrameList and needs to replace least recently used page
			{
				for(j = 0; j < FrameNR; j++)
				{
					if(lruPage[j] < oldestPage)
					{
						oldestPage = lruPage[j];
						indexOfOldestPage = j; 
					}
				}
				
				//replace old page with new page
				memory.PageFrameList[indexOfOldestPage] = accessPattern[i]; 
				lruPage[indexOfOldestPage] = i; 
				pageFaults++; 
			}
			else	//page was found in PageFrameList so all that needs to be done is update lruPAge array
			{
				lruPage[recentlyUsedPage] = i; 	
			}
		}
		printPageFrame(); 	
	}

	free(lruPage); 
	return pageFaults; 	
}

