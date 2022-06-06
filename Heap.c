#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// structure of node for allocated list
typedef struct node_taga
{
	char name[10];
	int address;
	int memory_alloc;
	struct node_taga *next;
    struct node_taga *prev;
}node_alloc;

// structure of node for free list
typedef struct node_tagf
{
	int start_address;
	int end_address;
	int memory_alloc;
	struct node_tagf *next;
  struct node_tagf *prev;
}node_free;

//making node of alloacted list
node_alloc* makingnode_alloc(char name[10],int add,int mem)
{
	node_alloc* ptr;
	ptr = (node_alloc*)malloc(sizeof(node_alloc));
	strcpy(ptr->name,name);
	ptr->address = add;
	ptr->memory_alloc = mem;
	ptr->next = NULL;
    ptr->prev = NULL;
	return ptr;
}


//making node of free list
node_free* makingnode_free(int sadd,int eadd,int mem)
{
	node_free* ptr;
	ptr = (node_free*)malloc(sizeof(node_free));
	ptr->start_address = sadd;
	ptr->end_address= eadd;
	ptr->memory_alloc = mem;
	ptr->next = NULL;
    ptr->prev = NULL;
	return ptr;
}

//insert node at start of allocated list
node_alloc* insertatstart_alloc(node_alloc *lptr,int add,int mem,char* name)
{
	node_alloc *ptr;
	ptr = makingnode_alloc(name,add,mem);
	if(lptr == NULL)
	{
		lptr = ptr;
		lptr->next = NULL;
		lptr->prev = NULL;
	}
	else
	{
		ptr->next = lptr;
    	lptr->prev = ptr;
    	ptr->prev = NULL;
		lptr = ptr;
	}
	return lptr;
}

//insert node at start of free list
node_free* insertatstart_free(node_free *lptr,int sadd,int eadd,int mem)
{
	node_free *ptr;
	ptr = makingnode_free(sadd,eadd,mem);
	if(lptr == NULL)
	{
		lptr = ptr;
		lptr->next = NULL;
		lptr->prev = NULL;
	}
	else
	{
		ptr->next = lptr;
    	lptr->prev = ptr;
    	ptr->prev = NULL;
		lptr = ptr;
	}
	return lptr;
}

//printing allocated list
void print_alloc(node_alloc *lptr)
{
	printf("This is the allocated list:\n");
	printf("Name\taddr\tmemory\n");
	while(lptr != NULL)
	{
		printf("%s\t",lptr->name);
		printf("%d\t",lptr->address);
		printf("%d\t",lptr->memory_alloc);
		lptr = lptr->next;
		printf("\n");
	}
}

//printing free list
void print_free(node_free *lptr)
{
	printf("This is the FreeList:\n");
	printf("start_addr\tEnd_addr\tmemory\n");
	while(lptr != NULL)
	{
		printf("%d\t\t",lptr->start_address);
		printf("%d\t\t",lptr->end_address);
		printf("%d\t",lptr->memory_alloc);
		lptr = lptr->next;
		printf("\n");
	}
}



// allocate memory and update the free list
node_alloc* allocate_firstfit(node_free **lptr,node_alloc *aptr,int memory,char name[10])
{
	node_free *ptr,*sptr,*previ;
	int start_address;
	int flag = 1;
	sptr = ptr = *lptr;
	while((ptr != NULL)&&(flag == 1))
	{
		if(ptr->memory_alloc >= memory)
		{
			start_address = ptr->start_address;
			ptr->start_address = (ptr->start_address) + memory;
			ptr->memory_alloc = (ptr->memory_alloc) - (memory);
			flag = 0;
			aptr = insertatstart_alloc(aptr,start_address,memory,name);
		}
		else
		{
			ptr = ptr->next;
		}
	}
	//if memory not available
	if(ptr == NULL)
	{
		printf("\n Oops,Memory can not be allocated \n");
	}
	//removing duplicate nodes with same start and end addresses
	while(sptr->next != NULL)
	{
		if((sptr->prev == NULL) && ((sptr->start_address) == (sptr->end_address)))
		{
			*lptr=(*lptr)->next;
			(*lptr)->prev=NULL;
			free(sptr);
		}
		else if(((sptr->next)->start_address) == ((sptr->next)->end_address))
		{
			previ = sptr;
			sptr = sptr->next;
			previ->next = sptr->next;
			if(sptr->next != NULL)
			{
				(sptr->next)->prev=previ;
			}
			free(sptr);
		}
		else
		{
			sptr = sptr->next;
		}
	}
	return aptr;
}

//free the memory
node_alloc* free_memory(node_alloc *aptr,node_free **fptr,char f[10])
{
	node_alloc *p,*sptr,*previ,*nptr;
	node_free *q,*optr;
	int start_address,end_address,memory_alloc,found = 0;
	q = *fptr;
	p = aptr;
	previ = NULL;
	while((p != NULL) && (found == 0))
	{
		if(!strcmp(p->name,f))
		{
			start_address = p->address;
			end_address = (p->address) + (p->memory_alloc);
			memory_alloc = p->memory_alloc;
			found = 1;
			if(p == aptr)
			{
				nptr = p;
				p = p->next;
				free(nptr);
				aptr = p;
        		aptr->prev = NULL;
			}
			else
			{
				previ->next = p->next;
       			(p->next)->prev = previ;
				free(p);
			}
		}
		else
		{
			previ = p;
			p = p->next;
		}
	}
	if(found == 0)
  	{
    	printf("----Enter proper name of the memory to free the memory:----\n");
  	}
	else
	{
		found = 0;
		while((q != NULL)&&(found == 0))
		{
			if(q->start_address == end_address)
			{
				q->start_address = start_address;
				q->memory_alloc = (q->memory_alloc) + (memory_alloc);
				found = 1;
			}
			else if(q->end_address == start_address)
			{
				q->end_address = end_address;
				q->memory_alloc = (q->memory_alloc) + (memory_alloc);
				found = 1;
			}
			q = q->next;
		}

		if(found == 0)
		{
			*fptr = insertatstart_free(*fptr,start_address,end_address,memory_alloc);
		}
	}
	return aptr;
}

//main function
int main()
{	node_alloc *aptr;
	node_free *fptr;
	int choice,flag = 1;
	char A[] = "abc";
	char B[] = "fvd";
	char C[] = "qwer";
	char D[] = "sdfo";
	char E[] = "fsdf";
	char F[] = "ejkl";
	printf("\n");
	printf(" HEAP MEMORY IS ASSUMED 1000 UNITS \n");
	printf("- - - - - - - - - - - - - - - - - - - - -\n");
	aptr = NULL;
	fptr = NULL;
	aptr = insertatstart_alloc(aptr,700,30,D);
	aptr = insertatstart_alloc(aptr,525,15,E);
	aptr = insertatstart_alloc(aptr,300,50,C);
	aptr = insertatstart_alloc(aptr,200,20,B);
	aptr = insertatstart_alloc(aptr,100,50,F);
	aptr = insertatstart_alloc(aptr,0,100,A);

	fptr = insertatstart_free(fptr,730,1000,270);
	fptr = insertatstart_free(fptr,540,700,160);
	fptr = insertatstart_free(fptr,350,525,175);
	fptr = insertatstart_free(fptr,220,300,80);
	fptr = insertatstart_free(fptr,150,200,50);

	while(flag == 1)
	{	printf("What operation do you want to perform \n");
		printf("1.Allocation of Memory \n");
		printf("2.Free up the memory \n");
		printf("3.Show the allocated memory and free memory \n");
		printf("4.Exit \n");
		printf("--------------------------------------------------\n");
		scanf("%d",&choice);
		if(choice == 4)
		{ 	flag = 0;
		}
		else if(choice == 3)
		{	print_alloc(aptr);
			print_free(fptr);
			printf("---------------------------------------------------\n");
			printf("Do you want to continue.press 1");
			scanf("%d",&flag);
		}
		else if(choice == 2)
		{	char fr[10];
			printf("Allocated and Free List before freeing memory:\n");
		    print_alloc(aptr);
		    print_free(fptr);
			printf("---------------------------------------------------");
			printf("Which one you want to free in allocated list enter that name:\n");
			scanf("%s",fr);
			aptr = free_memory(aptr,&fptr,fr);
			print_alloc(aptr);
			print_free(fptr);
			printf("---------------------------------------------------\n");
			printf("Do you want to continue.press 1\n");
			scanf("%d",&flag);
		}
		else
		{	int memory;
		    char name[10];
		    printf("How much memory do you want to allocate\n");
			scanf("%d",&memory);
			printf("Give the name of the process\n");
			scanf("%s",name);
			printf("Allocated and Free List before allocating memory:\n");
			print_alloc(aptr);
			print_free(fptr);
			aptr = allocate_firstfit(&fptr,aptr,memory,name);
			printf("\nAllocated and Free list after allocating memory;\n");
			print_alloc(aptr);
			print_free(fptr);
			printf("---------------------------------------------------\n");
			printf("Do you want to continue.press 1");
			scanf("%d",&flag);
		}
	}
	return 0;
}
