#include "client.h"

void sendFriendRequest(char *str)
{
	char **control_msg = g_strsplit(str, "-", 0);
	char *friend_name = control_msg[1];
	
	write(sfd, g_strjoin("-", CONTROL_MESSAGE, FRND_REQ, friend_name, NULL), MAXLINE);
}

void *client_out_handler(void *arg)
{
	int ret;
	char *input_buffer = (char *) malloc(MAXLINE);
	
	while(1)
	{
		ret = read(0, input_buffer, MAXLINE);
		input_buffer[ret - 1] = '\0';
		
		if(strcmp(input_buffer, LIST_REQ) == 0)
		{
			write(sfd, g_strjoin("-", CONTROL_MESSAGE, LIST_REQ, NULL), MAXLINE);
		}
		else if(g_strv_contains((const char * const*) g_strsplit(input_buffer, "-", 0), FRND_REQ))
		{
			sendFriendRequest(input_buffer);
		}
		else if(strcmp(input_buffer, EXIT_CHAT) == 0)
		{
			write(sfd, g_strjoin("-", CONTROL_MESSAGE, EXIT_CHAT, NULL), MAXLINE);
			exit(0);
		}
		else if(friend_connected)
		{
			write(sfd, input_buffer, MAXLINE);
		}
		else
		{
			printf("\nNo connection established\n\n");
		}
	}
}
