#include "server.h"

void nameRequest(char *str, int cfd, client_info *c_info)
{
	char **control_msg = g_strsplit(str, "-", 0);
	char *client_name = control_msg[2];
	
	c_info->username = g_strdup(client_name);
	c_info->cfd = cfd;
}

void getFriendInfo(char *str, int cfd, int *ffd, char **f_name)
{
	char **control_msg = g_strsplit(str, "-", 0);
	char *friend_name = control_msg[2];
	client_info *value;
	
	/* Critical section */
	g_mutex_lock(&hash_lock);
	GList *keys = g_hash_table_get_keys(hash);
	while(keys != NULL)
	{
		char *key = (char *) keys->data;
		value = (client_info*) g_hash_table_lookup(hash, key);
		if(strcmp(value->username, friend_name) == 0)
		{
			*ffd = value->cfd;
			*f_name = g_strdup(friend_name);
			write(cfd, g_strjoin("-", CONTROL_MESSAGE, FRIEND_RESPONSE_SUCCESS, friend_name, NULL), MAXLINE);
			g_mutex_unlock(&hash_lock);
			return;
		}
		keys = keys->next;
	}
	g_mutex_unlock(&hash_lock);
	write(cfd, g_strjoin("-", CONTROL_MESSAGE, FRIEND_RESPONSE_FAILURE, NULL), MAXLINE);
}

void sendClientList(int cfd)
{
	client_info *value;
	GString *client_list = g_string_new(CONTROL_MESSAGE);
	client_list = g_string_append(client_list, "-");
	client_list = g_string_append(client_list, LIST_RESPONSE);
	client_list = g_string_append(client_list, "-");
	
	/* Critical section */
	g_mutex_lock(&hash_lock);
	GList *keys = g_hash_table_get_keys(hash);
	
	while(keys != NULL)
	{
		char *key = (char *) keys->data;
		value = (client_info*) g_hash_table_lookup(hash, key);
		client_list = g_string_append(client_list, value->username);
		client_list = g_string_append(client_list, "\n");
		keys = keys->next;
	}
	g_mutex_unlock(&hash_lock);
	write(cfd, client_list->str, MAXLINE);
}

int isFriendOnline(char *f_name)
{
	client_info *value;
	
	/* Critical section */
	g_mutex_lock(&hash_lock);
	GList *keys = g_hash_table_get_keys(hash);
	
	while(keys != NULL)
	{
		char *key = (char *) keys->data;
		value = (client_info*) g_hash_table_lookup(hash, key);
		if(strcmp(value->username, f_name) == 0)
		{
			g_mutex_unlock(&hash_lock);
			return 1;
		}
		keys = keys->next;
	}
	g_mutex_unlock(&hash_lock);
	return 0;
}

void exitChat(int cfd, char *tid)
{
	/* Critical section */
	g_mutex_lock(&hash_lock);
	g_hash_table_remove(hash, tid);
	g_mutex_unlock(&hash_lock);
	
	/* Close thread and file descriptor */
	close(cfd);
	pthread_exit(NULL);
}

void *client_handler(void *arg)
{
	int cfd, ffd = -1;
	char *tid, *f_name;
	client_info *c_info;
	
	char *buffer = (char *) malloc(MAXLINE);
	
	cfd = *(int *) arg;
	
	/* Client info structure */
	c_info = (client_info *) malloc(sizeof(client_info));
	
	/* Insert to hash table - Critical section */
	g_mutex_lock(&hash_lock);
	tid = g_strdup_printf("%lu", pthread_self());
	g_hash_table_insert(hash, tid, c_info);
	g_mutex_unlock(&hash_lock);
	
	while(1)
	{
		memset(buffer, 0, MAXLINE);
		if(read(cfd, buffer, MAXLINE) <= 0)
		{
			exitChat(cfd, tid);
		}
		
		if(g_strv_contains((const char * const*) g_strsplit(buffer, "-", 0), CONTROL_MESSAGE))
		{
			printf("READ : %s\n", buffer);
			
			if(g_strv_contains((const char * const*) g_strsplit(buffer, "-", 0), NAME_REQUEST))
			{
				nameRequest(buffer, cfd, c_info);
			}
			else if(g_strv_contains((const char * const*) g_strsplit(buffer, "-", 0), FRND_REQ))
			{
				getFriendInfo(buffer, cfd, &ffd, &f_name);
			}
			else if(g_strv_contains((const char * const*) g_strsplit(buffer, "-", 0), LIST_REQ))
			{
				sendClientList(cfd);
			}
			else if(g_strv_contains((const char * const*) g_strsplit(buffer, "-", 0), EXIT_CHAT))
			{
				exitChat(cfd, tid);
			}
		}
		else
		{
			if(isFriendOnline(f_name))
			{
				write(ffd, g_strjoin("-", c_info->username, buffer, NULL), MAXLINE);
			}
			else
			{
				ffd = -1;
				f_name = NULL;
				write(cfd, g_strjoin("-", CONTROL_MESSAGE, FRIEND_OFFLINE, NULL), MAXLINE);
			}
		}
	}
}	
