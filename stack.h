#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <math.h>
#define me_calloc calloc
#define SIZE 128
typedef char chunk[SIZE];
char *bitmap;
chunk *chunks;
size_t n_chunks;

void me_init()
{
	n_chunks = pow(2, 16);
	void *ptr = sbrk(n_chunks * sizeof(chunk) + n_chunks);
	chunks = (chunk *)ptr;
	bitmap = (char *)((char *)(ptr) + n_chunks * sizeof(chunk));
	for (int i = 0; i < n_chunks; i++)
	{
		bitmap[i] = 0;
	}
}

void *me_malloc(size_t how_much)
{
	int sum;
	if (how_much % SIZE == 0)
	{
		sum = how_much / SIZE;
	}
	else
	{
		sum = how_much / SIZE + 1;
	}
	int counter = 0;
	int i = 0;
	int j = 0;
	while (counter < sum && i < n_chunks)
	{
		if (bitmap[i] == 0)
		{
			counter++;
		}
		else
		{
			j = i + 1;
			counter = 0;
		}
		i++;
	}
	if (counter < sum)
	{
		return NULL;
	}
	else
	{
		int a = 1;
		for (i = j; i < j + sum; i++)
		{
			bitmap[i] = a;
			a++;
		}
		return chunks[j];
	}
}

void *calloc(unsigned long num, unsigned long size_of_1)
{
	size_t how_much = num * size_of_1;
	unsigned long sum;
	if (how_much % SIZE == 0)
	{
		sum = how_much / SIZE;
	}
	else
	{
		sum = how_much / SIZE + 1;
	}
	int counter = 0;
	int i = 0;
	int j = 0;
	while (counter < sum && i < n_chunks)
	{
		if (bitmap[i] == 0)
		{
			counter++;
		}
		else
		{
			j = i + 1;
			counter = 0;
		}
		i++;
	}
	if (counter < sum)
	{
		return NULL;
	}
	else
	{
		int a = 1;
		for (i = j; i < j + sum; i++)
		{
			bitmap[i] = a;
			a++;
		}
		memset(chunks[j], 0, sum * SIZE);
		return chunks[j];
	}
}

void me_free(void *poi)
{
	int i = ((char *)(poi) - (char *)(chunks)) / SIZE;
	int j = 1;
	while (bitmap[i] == j)
	{
		bitmap[i] = 0;
		j++;
		i++;
	}
}

static int size = 0;
typedef struct stack
{
	char *str;
	struct stack *next;
} stack;

void push(stack **head, char *add)
{
	if (*head == NULL)
	{
		*head = (stack *)me_malloc(sizeof(stack));
		// if(head==NULL){
		//     //exit(1)
		// }
		stack *h = *head;
		h->str = (char *)me_malloc(strlen(add) + 1);
		if (h->str == NULL)
		{
			// exit(1);
		}
		strcpy(h->str, add);
		h->next = NULL;
		size++;
	}
	else
	{
		stack *t = (stack *)me_malloc(sizeof(stack));
		t->next = *head;
		if (t == NULL)
		{
			exit(1);
		}
		t->str = (char *)me_malloc(strlen(add) + 1);
		if (t->str == NULL)
		{
			exit(1);
		}
		strcpy(t->str, add);
		*head = t;
		size++;
	}
}
char *pop(stack **head)
{
	stack *h = *head;

	char *empty = (char *)me_malloc(15);
	empty = "stack is empty";
	if (h == NULL)
	{
		return empty;
	}
	else
	{
		stack *pr = h->next;
		char *st = h->str;
		me_free(h);
		*head = pr;
		printf("%s\n", st);
		size--;
		return st;
	}
}
const char *show(stack **head)
{
	stack *h = *head;
	if (h == NULL)
	{

		return "stack is empty";
	}
	else
	{
		return h->str;
	}
}
void print_stack(stack **head)
{
	int i = 0;
	while (*head != NULL)
	{
		printf("place %d:", i);
		pop(head);

		i++;
	}
}

// int main()
// {
//     stack *head=NULL;
//     while (1){
//         /* code */
//     printf("enter a command\n");
//     char *command = NULL;
//     // size of the command
//     size_t len = 0;
//     // size of the chars we got from stdin
//     ssize_t lineSize = 0;
//     lineSize = getline(&command, &len, stdin);
//     command[lineSize - 1] = '\0';
// if(strncmp("PUSH",command,4)==0){
// char t[1024];
// strcpy(t,command+5);
//  push(&head,t);
//  printf("was pushed successfully \n");
// }
// else if(strncmp("POP",command,3)==0){
//   pop(&head);

// }
// else if(strncmp("TOP",command,3)==0){
//   printf("%s\n",show(&head));
// }
// else if(strcmp("EXIT",command)==0){
//     print_stack(&head);
//     free(command);
//     break;
// }
// else if(strcmp("SIZE",command)==0){
//     printf("%d\n",size);
// }
//     free(command);
//     }
//     return 0;
// }
