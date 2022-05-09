/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>
#include <signal.h>
#include "stack.h"
#define SA struct sockaddr
#define PORT 3490 // the port users will be connecting to
#define MAX 1024
#define BACKLOG 10 // how many pending connections queue will hold
size_t n_connected = 0;

pthread_mutex_t lock;
stack *head = NULL;
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void *handle_client(void *args)
{
	int new_fd = *(int *)(args);
	char buff[MAX];
	// infinite loop for chat
	bzero(buff, MAX);
	while (read(new_fd, buff, sizeof(buff)))
	{
		printf("%s ", buff);
		if (strncmp("PUSH ", buff, 5) == 0)
		{
			char t[1024];
			strcpy(t, buff + 5);
			pthread_mutex_lock(&lock);
			push(&head, t);
			pthread_mutex_unlock(&lock);
			printf("was pushed successfully \n");
			char *buffer = "success";
			write(new_fd, buffer, sizeof(buffer));
		}
		else if (strncmp("POP", buff, 3) == 0)
		{
			pthread_mutex_lock(&lock);
			char *send_to_client = pop(&head);
			pthread_mutex_unlock(&lock);
			write(new_fd, send_to_client, strlen(send_to_client) + 1);
			me_free(send_to_client);
		}
		else if (strncmp("TOP", buff, 3) == 0)
		{
			char const *send_to_client1 = show(&head);
			write(new_fd, send_to_client1, strlen(send_to_client1) + 1);
		}
		else if (strcmp("EXIT", buff) == 0)
		{
			n_connected--;
			printf("%ld Users Connected!\n", n_connected);
			if (n_connected == 0)
			{
				printf("No more clients are connected, dumping stack memory\n");
				print_stack(&head);
			}
			break;
		}
		else if (strcmp("SIZE", buff) == 0)
		{
			printf("%d\n", size);
		}
		else
		{
			char *send_to_client2 = "illegal command";
			write(new_fd, send_to_client2, strlen(send_to_client2) + 1);
		}
		bzero(buff, MAX);
	}

	close(new_fd);
	char ref[] = "closed socket";
	pthread_exit(ref);
}

int main(int argc, char const *argv[])
{
	me_init();
	 int sockfd, new_fd, len;
    struct sockaddr_in servaddr, cli;
   
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
   
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
	if (pthread_mutex_init(&lock, NULL) != 0)
	{
		printf("\n mutex init failed\n");
		return 1;
	}
	while (1)
	{ // main accept() loop
		new_fd = accept(sockfd, (SA*)&cli, &len);
		if (new_fd == -1)
		{
			perror("accept");
			continue;
		}
		n_connected++;
		printf("server: got connection\n");

		// if (!fork()) { // this is the child process
		//     close(sockfd); // child doesn't need the listener
		// if (send(new_fd, "Hello, world!", 13, 0) == -1)
		// perror("send");
		// close(new_fd);
		// exit(0);
		// }
		pthread_t thr;
		pthread_create(&thr, NULL, handle_client, (void *)&new_fd);
		// close(new_fd);  // parent doesn't need this
	}
	pthread_mutex_destroy(&lock);

	return 0;
}