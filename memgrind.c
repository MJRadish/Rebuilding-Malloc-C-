#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.c"


int main(int argc, char** argv){

	int indexCounter;	//used for a general index counter when freeing every element in a array
	int loopCounter = 0;	//used to keep track of how many loop iterations have occured

	double avgtestF = 0;	
	double avgtestE = 0;
	double avgtestD = 0;	//variables that keep track of the time each memgrind test took
	double avgtestC = 0;	
	double avgtestB = 0;
	double avgtestA = 0;

  	struct timeval start, end; //used for gettimeofday
	while(loopCounter < 100){ //start of the loop, will loop each memgrind test 100 times
		
					//This is the start of the A memgrind test

		gettimeofday(&start, NULL); 				//start of A's time

		deleteAllMem(); 					//clearing out memory for memgrind A
		for(indexCounter = 0; indexCounter < 150; indexCounter += 1){
		char* A = (char*)malloc(1 * sizeof(char)); 		//creating then freeing the char pointer A
		free(A);
		
		}	
	 	gettimeofday(&end, NULL);				//end of A's time
		avgtestA += (double)(end.tv_sec - start.tv_sec)+ ( (double)(end.tv_usec - start.tv_usec) /1000000.0 ); //adding the time A took
	
		//The end of A memgrind test
		//This is the start of the B memgrind test
	gettimeofday(&start, NULL); 					//start of B's time	
	deleteAllMem();		  					 //clearing out memory for memgrind B
	char* timeArrC[150];						//the char pointer array that is used to store all the allocated pointers
	for(indexCounter = 0; indexCounter < 150; indexCounter += 1){			
		timeArrC[indexCounter] = (char*)malloc(1 * sizeof(char)); //first allocated 150 char pointers
		}
	for(indexCounter = 0; indexCounter < 150; indexCounter += 1){
		free(timeArrC[indexCounter]);				//frees the recently allocated 150 char pointers in sequential order
		}
	gettimeofday(&end, NULL);					//end of B's time
	avgtestB += (double)(end.tv_sec - start.tv_sec)+ ( (double)(end.tv_usec - start.tv_usec) /1000000.0 ); //adding the time B took
	
	//The end of B memgrind test
	//The start of C memgrind test
    gettimeofday(&start, NULL);					//start of C's time
  
   int mallocCount;			//used to determine how many mallocs have been performed->need 150 mallocs				
   int mallocIndex = -1;		//used to determine what index to malloc or free in the char* array
   int random;				//used to see either a free or malloc is performed based on a rng valueb
   		
   
   for( mallocCount = 0 ; mallocCount < 150  ; ) {
      random = rand() % 2; 			//random takes on randomly the value 0 or 1
   
	if(random == 1){ 			//when random is 1 then a system malloc is performed
		mallocIndex += 1;		//incrementing the index since the next ++ index is empty and needs a address
		timeArrC[mallocIndex] = (char*)malloc(1*sizeof(char));  
		mallocCount += 1;		//incrementing count since a sucessful malloc has been performed
			}
	else{					//random was 0 so a free is performed
	    if(mallocIndex < 0)			//base case for when you have no pointers to free//
		continue;
	    else{
		free(timeArrC[mallocIndex]);	//Have pointers to free so the system frees the index associated with mallocIndex
						
		mallocIndex -= 1;		//decrements mallocIndex b/c the current index is no longer valid, mallocIndex always has
		}				//to point to a valid index
		}
	}
	
	for(indexCounter = mallocIndex; indexCounter >= 0; indexCounter -= 1){
		free(timeArrC[indexCounter]);		//For loop frees the left over address b/c 150 mallocs have been performed
	}   
	
	gettimeofday(&end, NULL);		//end of C's time
	avgtestC += (double)(end.tv_sec - start.tv_sec)+ ( (double)(end.tv_usec - start.tv_usec) /1000000.0 ); //adding the time C took
	//End of memgrind test C
	//Start of memgrind test D
	
  
   gettimeofday(&start, NULL);	//start of D's time
   deleteAllMem();		//clearing memory for test D
   int randomMalloc;		//used to determine the amount of bytes to malloc base on rng value of 1-64
   mallocCount = 0;		//resetting both of the following variables to default value
   mallocIndex = -1;
  
   
   for( mallocCount = 0 ; mallocCount < 150  ; ) { //Once 150 sucessful mallocs have been performed, the system will break out of this loop
      random = rand() % 2; 			  //random will be either 1 or 0
      
	if(random == 1){ 			//random is 1 so a system malloc is performed
		mallocIndex += 1;
		randomMalloc = (rand() % 64) + 1; //the system seeing how many bytes to malloc base on a rng

		timeArrC[mallocIndex] = (char*)malloc(randomMalloc);	//mallocing the said random value now

		if(timeArrC[mallocIndex] == NULL){ //base case to see if the system had ran out of memory
			mallocCount -= 1;		//decrements both variables thus the system treats this malloc like it never happened
			mallocIndex -= 1;
			}
		mallocCount += 1;		//sucessful malloc has been performed, if the malloc return NULL, the if statment above    
			}			//neutralizes this statment
	else{
	    if(mallocIndex < 0)			//base case for when you have no pointers to free
		continue;
	    else{
		free(timeArrC[mallocIndex]);	//Have pointers to free so a sucessful free is performed, the next valid index is set to the
		mallocIndex -= 1;               //previous index value
		}
		}
	}
	
	for(indexCounter = mallocIndex; indexCounter >= 0; indexCounter -= 1){	
		free(timeArrC[indexCounter]);		//frees the left over address in the char pointer array
	}   
	
	gettimeofday(&end, NULL);			//end of D's time
	avgtestD += (double)(end.tv_sec - start.tv_sec)+ ( (double)(end.tv_usec - start.tv_usec) /1000000.0 ); //adding the time D took

	//End of memgrind test D
	//Start of memgrind test E	

 
  gettimeofday(&start, NULL);	//start of E's time
  deleteAllMem();		//clearing memory for memgrind test E
   
  mallocCount = 0;		//setting variables back to default values to be used again
  mallocIndex = -1;
   
   for( mallocCount = 0 ; mallocCount < 150  ; ) { //Loop breaks after 150 sucessful mallocs have been performed
      random = rand() % 2; 			   //random will be either 0 or 1
      
	if(random == 1){ 			//random is 1 so a malloc is performed
		mallocIndex += 1;
						
		randomMalloc = (rand() % 501) + 500; //determines the amount of bytes to malloc-> 500-1000
		timeArrC[mallocIndex] = (char*)malloc(randomMalloc);

		if(timeArrC[mallocIndex] == NULL){ 	//Ran out of memory, thus the system treats this malloc like it never happened
			mallocCount -= 1;		//decrementing these variables to neutralize the increments that happen with a 
			mallocIndex -= 1;		//sucessful malloc
			}
		mallocCount += 1;
			}
	else{						//random is 0 so a free is performed
	    if(mallocIndex < 0)				//base case for when you have no pointers to free, so nothing happens
		continue;
	    else{
		free(timeArrC[mallocIndex]);		//frees the a valid pointer, mallocIndex is set the the next valid pointer
		mallocIndex -= 1;
		}
		}
	}
	for(indexCounter = mallocIndex; indexCounter >= 0; indexCounter -= 1){	//frees the leftover points in the char* array
		free(timeArrC[indexCounter]);
	}   
	
	gettimeofday(&end, NULL); 					//End of memgrind test E's time		
	avgtestE += (double)(end.tv_sec - start.tv_sec)+ ( (double)(end.tv_usec - start.tv_usec) /1000000.0 ); //adding the time E took

	//End of memgrind test E
	//Start of memgrind test F	

  gettimeofday(&start, NULL); 				//start of test F's time
  deleteAllMem();					//clearing out memory for memgrind test F

  int* timeArrInt[150];					//creating an int pointer array and a double pointer array since test F allocates
  double* timeArrDouble[150];				//3 different data types and need an array to store each corresponding data type address
							//timeArrC is still used for the char pointer array
  mallocCount = 0;					//setting variables to default values
  mallocIndex = -1;
  int mallocIndexInt = -1;		
  int mallocIndexDouble = -1;				//creating index counter variables for their corresponding data type arrays
  							//allows the system to keep track of where a valid index point is located in these arrays
   							//mallocIndex is used for the char pointer array

   for( mallocCount = 0 ; mallocCount < 150  ; ) {	//loop breaks when a sucessful 150 mallocs have been performed
      random = rand() % 2; 				//radom with be either 0 or 1
     
	if(random == 1){ 			//random is 1, so a system malloc will be performed
		
		random = rand() % 3;   	 //random now will be either 0,1,or 2. This is used for the system to decided what data type to malloc
							//0 for char pointer, 1 for int pointer, or 2 for double pointer

		if(random == 0){			//random was 0, so the system will attempt to malloc a char pointer
			mallocIndex += 1;
			randomMalloc = (rand() % 64) + 1;  	//system decides what size to allocate->value will be 1-64 bytes
			timeArrC[mallocIndex] = (char*)malloc(randomMalloc);
			if(timeArrC[mallocIndex] == NULL){ 	//base case for when the system ran out of memory
				mallocCount -= 1;	   	//decrementing these variables tells the system that this malloc never happened
				mallocIndex -= 1;
			}
			}
		else if(random == 1){				//system decided to malloc a integer pointer instead
			mallocIndexInt += 1;			//this else if works exactly the same as the char pointer case
			randomMalloc = (rand() % 64) + 1; 
			timeArrInt[mallocIndexInt] = (int*)malloc(randomMalloc*sizeof(int));
			if(timeArrInt[mallocIndexInt] == NULL){
				mallocCount -= 1;
				mallocIndexInt -= 1;
			}
			}
		else{ 						//system decided to malloc a double pointer instead
		        mallocIndexDouble += 1;			//this else statement works exactly the same aas the char pointer case
			randomMalloc = (rand() % 64) + 1; 
			timeArrDouble[mallocIndexDouble] = (double*)malloc(randomMalloc*sizeof(double));
			if(timeArrDouble[mallocIndexDouble] == NULL){
				mallocCount -= 1;
				mallocIndexDouble -= 1;
			}
			}
	
		mallocCount += 1;
			}
	else{						//the system decided to attempt a free instead of a malloc
	    random = rand() % 3; 			//the way the system decides what data type to free	
	   						//0 for char pointer, 1 for int pointer, or 2 for double pointer

	  if(random == 0){				//random was 0 so a free will be attempted on the char pointer array
	    if(mallocIndex < 0)				//base case for when the char array has no pointers to free, the loop continues like
		continue;				//nothing happened
	    
	  else{						//base case not triggered, so a sucessful free will be performed on the char pointer array
		free(timeArrC[mallocIndex]);		//makes the mallocIndex, the index associated to the char array, point to the next valid 
		mallocIndex -= 1;			//index
				}
			}

	  else if(random == 1){				//random was 1 so a free will be attempted on the int pointer array
	      if(mallocIndexInt < 0)			//this process works exactly the same as attempting to free a character pointer
			continue;
	       else{
		free(timeArrInt[mallocIndexInt]);
		mallocIndexInt -= 1;
		}
			}

	  else{						//random was 2 so a free will be attempted on the double pointer array
	      if(mallocIndexDouble < 0)			//this process works exactly thesame as attempting to free a character pointer
			continue;
	       else{
		free(timeArrDouble[mallocIndexDouble]);
		mallocIndexDouble -= 1;
		}
			}	
		}
	}
											//the 3 following for loops free the left over addresses
	for(indexCounter = mallocIndex; indexCounter >= 0; indexCounter -= 1){		//associated with that data type array
		free(timeArrC[indexCounter]);
	}   
	for(indexCounter = mallocIndexInt; indexCounter >= 0; indexCounter -= 1){	
		free(timeArrInt[indexCounter]);
	}  
	for(indexCounter = mallocIndexDouble; indexCounter >= 0; indexCounter -= 1){	
		free(timeArrDouble[indexCounter]);
	}  
	
	gettimeofday(&end, NULL);										//end of F's time
	avgtestF += (double)(end.tv_sec - start.tv_sec)+ ( (double)(end.tv_usec - start.tv_usec) /1000000.0 );  //adding the time F took

	loopCounter++;					//tells the system that an additional loop happened for all memgrind test cases

		}			//end of the while loop
				
	printMem();  //REMOVE FROM FINAL

	deleteAllMem();	//freeing everything left over in memory
	
				//prints out the avg time that each memgrind test case took		

	printf("average time in seconds for case A: %lf\naverage time in seconds for case B: %lf\naverage time in seconds for case C: %lf\naverage time in seconds for case D: %lf\naverage time in seconds for case E: %lf\naverage time in seconds for case F: %lf		  \n",avgtestA/100.0,avgtestB/100.0 ,avgtestC/100.0,avgtestD/100.0,avgtestE/100.0 ,avgtestF/100.0);


	printf("check: %d loopcounter:%d\n",check,loopCounter); //REMOVE FROM FINAL 

	return 0;
}

