#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"

#define CHNKS 100 // a word big enough to take this amout of chunks in our simple implementation

const size_t word_len = CHNKS * SIZE;

int main(int argc, char const *argv[])
{
	me_init();
	stack *head = NULL;
	char big_word[word_len];
	memset(big_word, 0, word_len + 1);

	for (size_t i = 0; i < word_len; ++i)
	{
		big_word[i] = ('a' + (i % ('z' - 'a' + 1)));
	}

	// check PUSH (and TOP)
	printf("----------------------------------------------------------------------------\n");
	push(&head, "");
	assert(strcmp(show(&head), "") == 0);
	push(&head, "hello world!");
	assert(strcmp(show(&head), "hello world!") == 0);
	push(&head, "welcome to the metaverse");
	assert(strcmp(show(&head), "welcome to the metaverse") == 0);
	push(&head, "hi there");
	assert(strcmp(show(&head), "hi there") == 0);
	push(&head, big_word);
	assert(strcmp(show(&head), big_word) == 0);
	printf("Done PUSH!\n");

	// check POP
	printf("----------------------------------------------------------------------------\n");
	char *temp;
	temp = pop(&head);
	assert(strcmp(temp, big_word) == 0);
	temp = pop(&head);
	assert(strcmp(temp, "hi there") == 0);
	temp = pop(&head);
	assert(strcmp(temp, "welcome to the metaverse") == 0);
	temp = pop(&head);
	assert(strcmp(temp, "hello world!") == 0);
	temp = pop(&head);
	assert(strcmp(temp, "") == 0);
	printf("Done POP!\n");

	printf("----------------------------------------------------------------------------\n");
	printf("\n\n");
	printf("----------------------------------------------------------------------------\n");
	printf("Tests Passed!\n");
	printf("----------------------------------------------------------------------------\n");
	return 0;
}
