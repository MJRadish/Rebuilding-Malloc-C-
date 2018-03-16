#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h"

int check; //REMOVE FROM FINAL VERSION

static char myblock[5000];  //This is the simulated memory used in my_malloc

void deleteAllMem(){ //Resets all 5000 bytes in myblock to -1, or the default value thus resesting the memory
	int count;
	for(count = 0; count < 5000 ; count ++){ //For loop that makes each byte -1
		myblock[count] = -1;
		}

	}
	
void insert(void* addPointer,char* addValuePointer){ //Adds the newly created node (addPointer is the Node's address) to the end of the LL
	Node * tmp = (Node*)addPointer;		     //makes the addPoint attribute point to addValuePointer or the address created by malloc
	tmp->addPoint = addValuePointer;
	tmp->next = NULL;
	tmp->freeFlag = 1;			   //Sets freeFlag to 1 since the address malloced is valid and not freed yet
	Node* ptr;
								     //&myblock[4] is the head of the LL thus ptr is used to iterate
	for(ptr = (Node*)(&myblock[4]) ; ptr!= NULL; ptr = ptr->next){//For loop loops through the LL until it finds the end, then it adds the
		if(ptr->next == NULL){				      //newly created Node onto the end
			ptr->next = tmp;
			return;			//end found and Node inserted so it can break out of the function now
			}
		}
}

void printMem(){ //Debug function that loops through each byte in memblock and prints out each value associated with that byte
	int i;
	for( i = 0; i < 5000; i ++){	//prints the character as an int
		printf("%d\n",myblock[i]);
		}
	}

int search(char* targetAdd){//returns 0 if not there, 1 if there then it changes the free value of that node//	
	    
	Node* ptr = (Node*)(&myblock[4]); //ptr now points to the head node
	for(ptr = ptr->next ; ptr!= NULL; ptr = ptr->next){//ptr not points to the next of head node b/c head node does not point to any value 								    address. For loop loops throught entire LL if need be

		if((long long int)ptr->addPoint == (long long int)targetAdd){//checks to see if Node's addPoint points to the argument
				ptr->freeFlag = 1; //sets freeflag to 1 meaning that its now valid//
				return 1;	   //Node was found sucess
				}
					}
		return 0;			//Node not found failure				
}

int searchFree(char* targetAdd){    //returns -1 if address not there, 0 if there/but freed, 1 there and address was valid
	Node* ptr = (Node*)(&myblock[4]); //ptr now points to the head Node
	
	for(ptr = ptr->next ; ptr!= NULL; ptr = ptr->next){ //Loops through entire LL if need be
		if((long long int)ptr->addPoint == (long long int)targetAdd){//checking to see if ptr's addPoint points to the argument
				if(ptr->freeFlag == 1){			//seeing if that address is valid or freed
					ptr->freeFlag = 0;//setting freeFlag to zero b/c this address is not freed
					return 1;	  //returns sucess
						}
				return 0; //The Node that pointed to this address had the freeFlag set to 0, or that address was already freed
				}
					}
		return -1;			//Address not found in the LL					
}

void my_freeExtra(void* pointers){ //Helper method for my_free()
	
	char * pointer = pointers; // setting so the system can increment address by 1

	int allocatedSize,currentByte; //allocatedSize is used to see how many bytes the allocated space that is being freed is taking
	allocatedSize = 0;	       //currentByte is used to iterate thourgh the memory offest info and help calculate the allocated size
	currentByte = -2;	       //start from two bytes above the pointer since the first is guaranteed to be a memory offest info
				       //the other bytes above are not guaranteed, which the while() below does

	allocatedSize += pointer[-1];	//Adds the first memory offest info since this will always be less than 127

	while(1){			//used to calculate the memory offset info of the allocated chunck of space
	    if(pointer[currentByte] == 127)//if the first memory offest byte is set to 127,or the cap that the byte can hold
	 	allocatedSize += 127;	//which means that theres another memory offest byte directly -1 away. Breaks when is sees
					//a number under 127, b/c this byte will always be the firstline info-the info telling how many lines
					//or bytes the memory offest infomation is taking

		else{			//value was less than 127 so this is the first line info instead of memory offest info
		   allocatedSize += pointer[currentByte]; //adds this to allocatedSize so the system later can free all the memory offest bytes too
		   break;		
			}			
		currentByte -= 1;		//decremeting since the system wants to go upwards in the array
		}
		

	pointer = pointer + currentByte;		 //pointer now points to the last byte in the allocated space
	for(currentByte = 0;currentByte <= allocatedSize; currentByte += 1){//Loop through the allocated space starting from the last 
		pointer[currentByte] = -1;			   // position and moving upwards towards the top,including the memory offest  
		}						   //lines default values thus deleting that allocated space and setting to the
}

void my_free(void* pointer,char* file,int line){

	long long int boundary1 = (long long int)&myblock[0];	//creating these two variables to check if the address sent belongs to memblock[]
	long long int boundary2 = (long long int)&myblock[5000];	

	if((long long int)pointer < boundary1 || (long long int)pointer > boundary2){ //seeing if pointer is in memblock[]
		printf("Error: pointer not allocated in my_malloc in file: %s at line: %d\n",file,line);//pointer not in memblock so error	
		return;
		}
	
	int test = searchFree(pointer); //calling seachFree() to check if the pointer was freed,valid,or even exist in the LL
	if( test == -1){		
		printf("Error: pointer not allocated in my_malloc in file: %s at line: %d\n",file,line);
		return;			   //Pointer not found meaning my_malloc was not used to create this pointer thus error			 
			}

	if( test == 0){			  //the pointer's Node's freeFlag was set to the free marker thus double free error
		printf("Error: attempted double free in file: %s at line: %d\n",file,line);
		return;
	}
	my_freeExtra(pointer); //calls a helper method to acutally free the address b/c the address is know known to be valid	
}

//#define free(X) my_free((X),__FILE__,__LINE__)

void* my_malloc( unsigned int size,char* file, int line){
		
	if(size == 0){		//base case for when the user tries to malloc 0 bytes, results in a error
		printf("Error: Can not allocate 0 bytes in file: %s line: %d\n",file,line);
		return NULL;
		}
				//the first byte tells the system if the head of the LL exist or not, when the first byte is not -5 thus the if
	if(myblock[0] != -5 ){	//statment is true, the system now creates a head Node firstbv and initializes the entire memblock  
		myblock[0] = -5; //to the default value. The head node is about to be created so the first byte is set to -5
		int memoryInitializer;
		for(memoryInitializer = 2; memoryInitializer < 5000; memoryInitializer ++){ //setting the entire memblock to default value of -1
			myblock[memoryInitializer] = -1;
			}
		myblock[1] = 1;       //when the 2nd byte is set to 1, the system now knows that a system Node is being created
				      //which stops a creation of a Node that points to that allocated space which would result in an infinite loop
		Node* test = (Node*)my_malloc(sizeof(Node),file,line); //mallocing the head node
		test->next = NULL;
		test->freeFlag = 1; //setting the head Node freeFlag to valid and next to NULL b/c there doesnt exist anyother Nodes yet
		}
		

	int memCounter;
	int freeSpaceCounter; //keeps track of the first unallocated byte found in memblock
	//int count;//used for the main algo to count of the unallocated space in memblock
	int firstLine;
	int memOffestLines; //used to write the memory offest info to a allocated space if found
	int dummySize;//used to calculate the memory offest info, need another variable that holds size's value since the system changes this var 
	int allocatedCount = 0; //used to determine if memblock has enough bytes in a unallocated space, used as a counter
	int firstLineInfo = 0;	//used to determine how many lines the memory offest info needs

	dummySize = size; 	//If dummySize is greater than 127, this means that more than 1 memory offest line is required so the system needs
				//to entire a loop. The loop subtracts 127 until dummySize is less than 127. For each subtract performed. The
	if(dummySize >= 127){ 	//allocated space required one more line for memory offest info 
		firstLineInfo += 1;	
		while(dummySize >= 127){		
			dummySize -= 127;	
			firstLineInfo += 1;  //subtract was sucessful thus system needs ++ more lines		
						
			}
			}
	else
	  firstLineInfo = 1; //base case for when size is less than 127, so the system just needs 1 line for the memory offest

								//start of the main algo that either allocates space or returns NULL
								//Loops through the entire myblock array if needed starting from 2 since
	for(memCounter = 2; memCounter < 5000 ; memCounter ++){ //this is the start of the memory offest Line of the head Node
								
								//means that it just hit an unallocated space in memory and needs to check the
		if((int)myblock[memCounter] == -1){     	//next (size + memOff +1) blocks in memory and sees if those are free, if so then
								//if allocates sucessfully, if it hits a non def value(meaning that the current	
					                	//chuck is allocated to something else), then it breaks out of the while loop
		                                       
			freeSpaceCounter = memCounter;          //Need to keep track of that first unallocated space found since if there is enough
			allocatedCount = 0;				//space below,this is where the First Line info goes

			while(memCounter < 5000){
			  allocatedCount += 1;	//incremeting since this is the amount of free bytes seen so far
			   if((int)myblock[memCounter] != -1)//the byte that memcounter is referencing is not free, so it has to break
				break;			     //out of this mini loop so the system can skip over the allocated space
	
			   if(allocatedCount == (int)(size + firstLineInfo + 1)){ //Enough memory has been found, added by 1 since need to 
								  //account for the spacethat the first line info needs
				myblock[freeSpaceCounter] = firstLineInfo; //making the first free byte the first Line info
				
				if(firstLineInfo== 1)		   	         //base case for when firstLineInfo is 1			
					myblock[freeSpaceCounter+1] = (int)size; //if so then the only memory offest info is just equal to the size

				else{					//there is more than 1 memory offest info
				for(memOffestLines = 1; memOffestLines < firstLineInfo; memOffestLines += 1){ //loop writes the memory offest info
												         //in the FirstLineInfo amout of lines
		
					myblock[freeSpaceCounter + memOffestLines] = 127; //sequential writes 127 to the assocaited bytes
											  //since 127 is the max value a byte can hold
					}
					myblock[freeSpaceCounter + firstLineInfo] = (int)size - (127 * (firstLineInfo -1)); //writes the last 															           //memory offest value
					}	
							     		
				//REMOVE FROM FINAL VERSION-> debug function			
			for(memCounter = 0 ; memCounter < (int)size ; memCounter ++){ 
					myblock[memCounter + freeSpaceCounter + firstLineInfo + 1] = -8;
					}
						
			
			if(myblock[1] == 0){ //if the 2nd byte is set to 0, then this means that the user called my_malloc so the system
					     //has to now malloc a associated Node that is responsible for this newly allocated space
				myblock[1] = 1;//setting the first byte to 1 since the system now is going to malloc a system Node

				int searchFlag = search(&myblock[freeSpaceCounter+firstLineInfo+1]); 
				//Before the system calls malloc and allocates a Node, the system calls search and sees if a Node already exist 				  that points to the newly allocated address, if so, a new Node is not needed, system reuses that old Node since 					  that address was previously freed and can now be used again
				 	
				if(searchFlag == 0){ //No Node exist, new Node must be created						     
												    
					Node* checkIfVoid = (Node*)my_malloc(sizeof(Node),file,line); //mallocing a new Node
				
					if(checkIfVoid == NULL){ //has to see if that new Node points to Node which means the system ran out of 
								 //space. If so, then the allocated chunk of memory that was done has to be 									 //reversed and NULL will be return to indicate that the system ran out of memory
					my_freeExtra((char*)&myblock[freeSpaceCounter+firstLineInfo+1]); //calls my_freeExtra b/c the newly 													allocated address doesnt have a corresponding Node
					return NULL;
							}
				insert(checkIfVoid,&myblock[freeSpaceCounter+firstLineInfo+1]); //inserts the Newly created Node in the LL
							}					//which points to the allocated space
					}

				myblock[1] = 0; //setting the 2nd byte back to 0 in preparation for another user call
				return &myblock[freeSpaceCounter+firstLineInfo+1]; //allocated sucessfully, returns the address of the first byte
										   //thats undernearth the memory offest info
					}
				memCounter++; //system increments to the next byte since none the above conditions were met  
				}
						}
		if(memCounter == 5000){//base case for when the system looped through the entire memblock and couldnt find enough space
			printf("Error: ran out of memory in File: %s at line: %d\n",file,line);	//prints out the associated error message
			check += 1; //REMOVE FROM FINAL VERSION	
			return NULL;
			}

		if((int)myblock[memCounter] != -1){	//found previously alloacted space, need to jump over this block of space//
			allocatedCount = 0; //reusing allocatedCount to count the memory offest info the previously allocated space
			
			for(firstLine = (int)myblock[memCounter]; firstLine > 0; firstLine -= 1){ //loops through the allocated space depending	
												  //of the first line value of that space	
				memCounter ++;
				allocatedCount += (int)myblock[memCounter];		//adds each memory offest info to allocatedCount
				}
			
				
			memCounter = memCounter + allocatedCount; // memCounter now points over that previously allocated space//
			
			
			}

	 								}
	//Ran through entire loop without finding enough memory, prints out associated error message and returns NULL
	printf("Error: ran out of memory in File: %s at line: %d\n",file,line);
	check += 1;//REMOVE FROM FINAL VERSION
	return NULL;  

}
//#define malloc(X) my_malloc((X),__FILE__,__LINE__)
