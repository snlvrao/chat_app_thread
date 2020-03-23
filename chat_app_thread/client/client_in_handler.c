#include "client.h"

void printClientList(char *str)
{
	char **response_msg = g_strsplit(str, "-", 0);
	char *client_list = response_msg[2];
	
	printf("\n%s\n", client_list);
}

void *client_in_handler(void *arg)
{
	char *socket_buffer = (char *) malloc(MAXLINE);
	
	while(1)
	{
		if(read(sfd, socket_buffer, MAXLINE) <= 0)
		{
			continue;
		}
		
		if(g_strv_contains((const char * const*) g_strsplit(socket_buffer, "-", 0), CONTROL_MESSAGE))
		{
			if(g_strv_contains((const char * const*) g_strsplit(socket_buffer, "-", 0), LIST_RESPONSE))
			{
				printClientList(socket_buffer);
			}
			else if(g_strv_contains((const char * const*) g_strsplit(socket_buffer, "-", 0), FRIEND_RESPONSE_SUCCESS))
			{
				printf("\nFriend connected\n\n");
				friend_connected = 1;
			}
			else if(g_strv_contains((const char * const*) g_strsplit(socket_buffer, "-", 0), FRIEND_RESPONSE_FAILURE) || 
			g_strv_contains((const char * const*) g_strsplit(socket_buffer, "-", 0), FRIEND_OFFLINE))
			{
				friend_connected = 0;
				printf("\nClient has gone offline\n\n");
			}
		}
		else
		{
			char **response_msg = g_strsplit(socket_buffer, "-", 0);
			
			printf("\n%s : %s\n\n", response_msg[0], response_msg[1]);
		}
	}
}
