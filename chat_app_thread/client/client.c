#include "client.h"

int main(int argc, char *argv[])
{
	int ret;

	typedef struct sockaddr_in sock_addr;
	sock_addr server_addr, client_addr;

	if(argc < 3)
	{
		printf("Insufficient arguments\n");
		exit(1);
	}

	/* Initialize server_addr struct members */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);

	/* Create TCP socket */
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1)
	{
		perror("socket");
		exit(1);
	}

	/* Connect to server */
	if(connect(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
	{
		perror("connect");
		exit(1);
	}
	
	/* Send credentials to server */
	write(sfd, g_strjoin("-", CONTROL_MESSAGE, NAME_REQUEST, argv[2], NULL), MAXLINE);
	
	/* Client options */
	printf("To retieve client list :   Enter LIST_REQ\n");
	printf("\nTo connect to client :   Enter FRND_REQ-<Name>\n");
	printf("\nTo exit chat :           Enter EXIT_CHAT\n\n");

	/* Spawn threads to handle incoming and outgoing messages */
	pthread_t tid_in;
	pthread_t tid_out;
	
	/* Thread attributes */
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	/* Create threads */
	if(pthread_create(&tid_in, &attr, client_in_handler, NULL) < 0)
	{
		perror("pthread");
		exit(1);
	}
	
	if(pthread_create(&tid_out, &attr, client_out_handler, NULL) < 0)
	{
		perror("pthread");
		exit(1);
	}
	
	pthread_join(tid_in, NULL);
	pthread_join(tid_out, NULL);
}
