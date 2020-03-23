#include "shared.h"

#define MAX_CONNECTIONS 4

typedef struct client_info
{
	char *username;
	int cfd;
}client_info;

GHashTable *hash;
GMutex hash_lock;

void *client_handler(void *arg);
