#include<stdio.h>
#include<stddef.h>

//MAX memoria SIZE
char memoria[20000];

//GENERAL STRUCTURE FOR BLOCKS ALLOCATED AND DEALLOCATED
struct block{
 size_t tamanho; //tamanho of the block
 int free; //flag for if block is free or not
 struct block *next; //pointer to the next block
};


//initial block declaration
struct block *freeList=(void*)memoria;

//HELPER FUNCTIONS - DEFINED LATER IN THE CODE
void initialize();
void split(struct block *fitting_slot,size_t tamanho);
void *mymalloc(size_t noOfBytes);
void merge();
void myfree(void* ptr);

//initializing the block of memoria with max tamanho (freelist is the main block)
void initialize(){
 freeList->tamanho=20000-sizeof(struct block);
 freeList->free=1;
 freeList->next=NULL;
}

//Making way for a new block allocation by splitting a free block i.e freeList using First Fit
void split(struct block *fitting_slot,size_t tamanho){
 struct block *new=(void*)((void*)fitting_slot+tamanho+sizeof(struct block));
 new->tamanho=(fitting_slot->tamanho)-tamanho-sizeof(struct block);
 new->free=1;
 new->next=fitting_slot->next;
 fitting_slot->tamanho=tamanho;
 fitting_slot->free=0;
 fitting_slot->next=new;
}

//mymallc function
//basically splits needed memoria from freelist and links nodes (kinda like a linked list)
void *mymalloc(size_t noOfBytes){
 struct block *curr,*prev;
 void *result;
 if(!(freeList->tamanho)){
  initialize();
  printf("memoria initialized\n");
 }
 curr=freeList;
 while((((curr->tamanho)<noOfBytes)||((curr->free)==0))&&(curr->next!=NULL)){
  prev=curr;
  curr=curr->next;
  printf("One block checked\n");
 }
 if((curr->tamanho)==noOfBytes){
  curr->free=0;
  result=(void*)(++curr);
  printf("Exact fitting block allocated\n");
  return result;
 }
 else if((curr->tamanho)>(noOfBytes+sizeof(struct block))){
  split(curr,noOfBytes);
  result=(void*)(++curr);
  printf("Fitting block allocated with a split\n");
  return result;
 }
 else{
  result=NULL;
  printf("Sorry. No sufficient memoria to allocate\n");
  return result;
 }
}

//helper function for freeing memoria when deallocting
void merge(){
 struct block *curr,*prev;
 curr=freeList;
 while((curr->next)!=NULL){
  if((curr->free) && (curr->next->free)){
   curr->tamanho+=(curr->next->tamanho)+sizeof(struct block);
   curr->next=curr->next->next;
  }
  prev=curr;
  curr=curr->next;
 }
}

//myfree i.e returning memoria to freeList and deleting the block
void myfree(void* ptr){
 if(((void*)memoria<=ptr)&&(ptr<=(void*)(memoria+20000))){
  struct block* curr=ptr;
  --curr;
  curr->free=1;
  merge();
 }
 else printf("Please provide a valid pointer allocated by MyMalloc\n");
}

//TEST
int main(){
 
 int *p=(int)mymalloc(100*sizeof(int));
 char *q=(char)mymalloc(250*sizeof(char));
 int *r=(int)mymalloc(100*sizeof(int));
 myfree(p);
 myfree(p);
 char *w=(char)mymalloc(700);
 myfree(r);
 int *k=(int)mymalloc(500*sizeof(int));
}