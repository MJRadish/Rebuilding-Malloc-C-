#ifndef MYMALLOC_H
#define MYMALLOC_H

void* my_malloc(unsigned int, char*, int);

#define malloc(X) my_malloc((X),__FILE__,__LINE__);

void my_free(void*,char*,int);

#define free(X) my_free((X),__FILE__,__LINE__);

typedef struct my_Node{
char* addPoint;	//the address that the Node points to, thus the address that my_malloc() returns to the user
char freeFlag;	//tells the system if a address is freed or not. Set to 1 if valid and set to 0 if address pointed to is freed
struct my_Node* next;
}Node;
void deleteAllMem();

void insert(void*,char*);

void my_freeExtra(void*);

void printMem();

int search(char*);

int searchFree(char*);






#endif
