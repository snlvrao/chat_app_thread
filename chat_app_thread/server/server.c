#include "server.h"

int main(int argc, char *argv[])
{
	int sfd, len;

	typedef struct sockaddr_in sock_addr;
	sock_addr server_addr, client_addr;

	if(argc < 2)
	{
		printf("Insufficient arguments\n");
		exit(1);
	}
	
	/* Server start message */
	printf("Server running...\n");
	
	/* Create hash table */
	hash = g_hash_table_new(g_str_hash, g_str_equal);
	
	/* Initialize mutex lock */
	g_mutex_init(&hash_lock);

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

	/* Bind server to socket */
	if(bind(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind");
		exit(1);
	}

	/* Listen to client connections on socket */
	if(listen(sfd, MAX_CONNECTIONS) == -1)
	{
		perror("listen");
		exit(1);
	}
	
	/* Accept connection requests on socket */
	len = sizeof(sock_addr);
	while(1)
	{	
		int cfd = accept(sfd, (struct sockaddr *) &client_addr, &len);
		if(cfd == -1)
		{
			perror("accept");
			exit(1);
		}
		
		/* Thread ID */
		pthread_t tid;
		
		/* Thread attributes */
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		
		/* Create thread */
		if(pthread_create(&tid, &attr, client_handler, &cfd) < 0)
		{
			perror("pthread");
			exit(1);
		}
	}
}
